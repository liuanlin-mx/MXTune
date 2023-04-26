/*
 * Copyright (C) 2021-2023 Liu An Lin <liuanlin-mx@qq.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <math.h>
#include <string.h>
#include "pitch_detector_talent.h"

#define PI (float)3.14159265358979323846
#define L2SC (float)3.32192809488736218171

pitch_detector_talent::pitch_detector_talent(float sample_rate)
    : pitch_detector()
    , _aref(440)
    , _conf(0)
    , _vthresh(0.7)
    , _gate(-60)
    , _pitch(0.)
    , _sample_rate(sample_rate)
    , _buf_size(ring_buffer::get_size_from_rate(sample_rate))
    , _corr_size(_buf_size / 2 + 1)
    , _max_period(1. / 70.)
    , _min_period(1. / 800.)
    , _max_idx((std::int32_t)(sample_rate * _max_period))
    , _min_idx((std::int32_t)(sample_rate * _min_period))
    
    , _cbwindow(NULL)
    , _ffttime(NULL)
    , _acwinv(NULL)
    , _complex(NULL)
    , _forward_plan()
    , _reverse_plan()
    
    , _buffer(sample_rate)
    , _noverlap(4)
    
{
    if (_max_idx > _corr_size)
    {
        _max_idx = _corr_size;
    }
    
    // Generate a window with a single raised cosine from N/4 to 3N/4
    _cbwindow = (float *)fftwf_malloc(_buf_size * sizeof(float));
    memset(_cbwindow, 0, _buf_size * sizeof(float));
    for(std::int32_t i = 0; i < (_buf_size / 2); i++)
    {
        _cbwindow[i + _buf_size / 4] = -0.5 * cos(4 * PI * i / (_buf_size - 1)) + 0.5;
    }
    
    
    _ffttime = (float *)fftwf_malloc(_buf_size * sizeof(float));
    _acwinv = (float *)fftwf_malloc(_buf_size * sizeof(float));
    _complex = (fftwf_complex *)fftwf_malloc(_corr_size * sizeof(fftwf_complex));
    _forward_plan = fftwf_plan_dft_r2c_1d(_buf_size, _ffttime, _complex, FFTW_ESTIMATE);
    _reverse_plan = fftwf_plan_dft_c2r_1d(_buf_size, _complex, _ffttime, FFTW_ESTIMATE);
    
    
    // ---- Calculate autocorrelation of window ----
    for(std::int32_t i = 0; i < _buf_size; i++)
    {
        _ffttime[i] = _cbwindow[i];
    }
    
    // Calculate FFT
    fftwf_execute(_forward_plan);
    
    // Remove DC
    _complex[0][0] = 0;
    _complex[0][1] = 0;
    

    // Take magnitude squared
    for(std::int32_t i = 1; i < _corr_size - 1; i++)
    {
        _complex[i][0] = (_complex[i][0]) * (_complex[i][0]) + (_complex[i][1]) * (_complex[i][1]);
        _complex[i][1] = 0;
    }
    
    
    // Calculate IFFT
    fftwf_execute(_reverse_plan);

    for(std::int32_t i = 1; i < _buf_size; i++)
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


pitch_detector_talent::~pitch_detector_talent()
{
    fftwf_destroy_plan(_forward_plan);
    fftwf_destroy_plan(_reverse_plan);
    fftwf_free(_complex);
    
    fftwf_free(_cbwindow);
    fftwf_free(_ffttime);
    fftwf_free(_acwinv);
}

void pitch_detector_talent::set_freq_range(float min_freq, float max_freq)
{
    _max_period = 1. / min_freq;
    _min_period = 1. / max_freq;
    _max_idx = (std::int32_t)(_sample_rate * _max_period);
    _min_idx = (std::int32_t)(_sample_rate * _min_period);
    
    if (_max_idx > _corr_size)
    {
        _max_idx = _corr_size;
    }
}

bool pitch_detector_talent::get_pitch(float in, float& pitch, float& conf)
{
    _buffer.put(in);
    if(_buffer.get_idx() % (_buffer.get_buf_size() / _noverlap) == 0)
    {
        pitch = _get_pitch(_buffer, conf);
        return true;
    }
    return false;
}

float pitch_detector_talent::_get_pitch(ring_buffer& buffer, float& conf)
{
    float db = 0;
    std::int32_t n = buffer.get_buf_size();
    // ---- Obtain autocovariance ----
    
    // Window and fill FFT buffer
    std::int32_t iwr = buffer.get_idx();
    for(std::int32_t i = 0; i < n; i++)
    {
        float v = buffer[(iwr - i + n) % n];
        _ffttime[i] = (float)(v * _cbwindow[i]);
        db = db + fabsf(v);
    }
    
    db = 20 * log10f(db / n);
    
    // Calculate FFT
    fftwf_execute(_forward_plan);
    
    // Remove DC
    _complex[0][0] = 0;
    _complex[0][1] = 0;
    

    // Take magnitude squared
    for(std::int32_t i = 1; i < _corr_size - 1; i++)
    {
        _complex[i][0] = (_complex[i][0]) * (_complex[i][0]) + (_complex[i][1]) * (_complex[i][1]);
        _complex[i][1] = 0;
    }
    
    
    // Calculate IFFT
    fftwf_execute(_reverse_plan);

    // Normalize
    float tf = (float)1 / _ffttime[0];
    for(std::int32_t i = 1; i < n; i++)
    {
        _ffttime[i] = _ffttime[i] * tf;
    }
    _ffttime[0] = 1;

    //  ---- END Obtain autocovariance ----



    //  ---- Calculate pitch and confidence ----

    // Calculate pitch period
    //   Pitch period is determined by the location of the max (biased)
    //     peak within a given range
    //   Confidence is determined by the corresponding unbiased height
    float tf2 = 0;
    std::int32_t ti4 = 0;
    float period = _min_period;
    for(std::int32_t i = _min_idx; i < _max_idx; i++)
    {
        std::int32_t i_left = i - 1;
        std::int32_t i_right = i + 1;
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
    
    if (db < _gate || tf < MIN_PITCH || tf > MAX_PITCH)
    {
        conf = 0;
    }
    
    if(conf >= _vthresh)
    {
        _pitch = tf;
    }
    _conf = conf;

    //  ---- END Calculate pitch and confidence ----

    return _pitch;
}
