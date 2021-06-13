#include <math.h>
#include <string.h>
#include "pitch_detector.h"
#include "mat_helper.h"

#define PI (float)3.14159265358979323846
#define L2SC (float)3.32192809488736218171

pitch_detector::pitch_detector(float sample_rate)
{
    _aref = 440;
    _conf = 0;
    _vthresh = 0.7;
    _pitch = 0.;
    
    _buf_size = ring_buffer::get_size_from_rate(sample_rate);
    _corr_size = _buf_size / 2 + 1;
    _sample_rate = sample_rate;
    _max_period = 1. / 70.;
    _min_period = 1. / 700.;
    _max_idx = (unsigned long)(sample_rate * _max_period);
    _min_idx = (unsigned long)(sample_rate * _min_period);
    
    if (_max_idx > _corr_size)
    {
        _max_idx = _corr_size;
    }
    
    
    
    // Generate a window with a single raised cosine from N/4 to 3N/4
    _cbwindow = (float *)calloc(_buf_size, sizeof(float));
    for(int i = 0; i < (_buf_size / 2); i++)
    {
        _cbwindow[i + _buf_size / 4] = -0.5 * cos(4 * PI * i / (_buf_size - 1)) + 0.5;
    }
    
    _fft = fft_con(_buf_size);
    
    _ffttime = (float *)calloc(_buf_size, sizeof(float));
    _fftfreqre = (float *)calloc(_corr_size, sizeof(float));
    _fftfreqim = (float *)calloc(_corr_size, sizeof(float));
    _acwinv = (float *)calloc(_buf_size, sizeof(float));
    
    
    
    // ---- Calculate autocorrelation of window ----
    for(int i = 0; i < _buf_size; i++)
    {
        _ffttime[i] = _cbwindow[i];
    }
    
    fft_forward(_fft, _cbwindow, _fftfreqre, _fftfreqim);
    
    for(int i = 0; i < _corr_size; i++)
    {
        _fftfreqre[i] = (_fftfreqre[i]) * (_fftfreqre[i]) + (_fftfreqim[i]) * (_fftfreqim[i]);
        _fftfreqim[i] = 0;
    }
    
    fft_inverse(_fft, _fftfreqre, _fftfreqim, _ffttime);
    
    for(int i = 1; i < _buf_size; i++)
    {
        _acwinv[i] = _ffttime[i] / _ffttime[0];
        if(_acwinv[i] > 0.000001)
        {
            _acwinv[i] = (float)1 / _acwinv[i];
        }
        else
        {
            _acwinv[i] = 0;
        }
    }
    _acwinv[0] = 1;
    // ---- END Calculate autocorrelation of window ----
    
}


pitch_detector::~pitch_detector()
{
    free(_cbwindow);
    free(_ffttime);
    free(_fftfreqre);
    free(_fftfreqim);
    free(_acwinv);
    fft_des(_fft);
}

float pitch_detector::get_period(ring_buffer& buffer, float& conf)
{
    int n = buffer.get_buf_size();
    // ---- Obtain autocovariance ----
    
    // Window and fill FFT buffer
    int iwr = buffer.get_idx();
    for(int i = 0; i < n; i++)
    {
        _ffttime[i] = (float)(buffer[(iwr - i + n) % n] * _cbwindow[i]);
    }

    //mat_helper_write_mat2_4("ffttime", 1, n, "float", _ffttime);
    //mat_helper_write_mat2_4("cbwindow", 1, n, "float", _cbwindow);
    
    // Calculate FFT
    fft_forward(_fft, _ffttime, _fftfreqre, _fftfreqim);

    //mat_helper_write_mat2_4("fftfreqre", 1, n, "float", _fftfreqre);
    //mat_helper_write_mat2_4("fftfreqim", 1, n, "float", _fftfreqim);
    
    // Remove DC
    _fftfreqre[0] = 0;
    _fftfreqim[0] = 0;

    // Take magnitude squared
    for(int i = 1; i < _corr_size; i++)
    {
        _fftfreqre[i] = _fftfreqre[i] * _fftfreqre[i] + _fftfreqim[i] * _fftfreqim[i];
        _fftfreqim[i] = 0;
    }
    
    //mat_helper_write_mat2_4("fftfreqre2", 1, n, "float", _fftfreqre);
    //mat_helper_write_mat2_4("fftfreqim2", 1, n, "float", _fftfreqim);
    
    // Calculate IFFT
    fft_inverse(_fft, _fftfreqre, _fftfreqim, _ffttime);

    // Normalize
    float tf = (float)1 / _ffttime[0];
    for(int i = 1; i < n; i++)
    {
        _ffttime[i] = _ffttime[i] * tf;
    }
    _ffttime[0] = 1;
    
    //mat_helper_write_mat2_4("ffttime2", 1, n, "float", _ffttime);
    //  ---- END Obtain autocovariance ----



    //  ---- Calculate pitch and confidence ----

    // Calculate pitch period
    //   Pitch period is determined by the location of the max (biased)
    //     peak within a given range
    //   Confidence is determined by the corresponding unbiased height
    float tf2 = 0;
    int ti4 = 0;
    float period = _min_period;
    for(int i = _min_idx; i < _max_idx; i++)
    {
        int i_left = i - 1;
        int i_right = i + 1;
        if(i_left < 0)
        {
            i_left = 0;
        }
        
        if(i_right > _corr_size)
        {
            i_right = _corr_size;
        }
        
        tf = _ffttime[i];

        if(tf > _ffttime[i_left] && tf >= _ffttime[i_right] && tf > tf2)
        {
            tf2 = tf;
            ti4 = i;
        }
    }
    
    conf = _conf;
    if(tf2 > 0)
    {
        conf = tf2 * _acwinv[ti4];
        if(ti4 > 0 && ti4 < _corr_size)
        {
            // Find the center of mass in the vicinity of the detected peak
            tf = _ffttime[ti4 - 1] * (ti4 - 1);
            tf = tf + _ffttime[ti4] * (ti4);
            tf = tf + _ffttime[ti4 + 1] * (ti4 + 1);
            tf = tf / (_ffttime[ti4 - 1] + _ffttime[ti4] + _ffttime[ti4 + 1]);
            period = tf / _sample_rate;
        }
        else
        {
            period = (float)ti4 / _sample_rate;
        }
    }


    // Convert to semitones
    tf = (float)-12 * log10((float)_aref * period) * L2SC;
    
    if(conf >= _vthresh)
    {
        _pitch = tf;
    }
    _conf = conf;

    //  ---- END Calculate pitch and confidence ----

    return _pitch;
}
