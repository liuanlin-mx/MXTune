#ifndef __PITCH_DETECTOR_H__
#define __PITCH_DETECTOR_H__
#include "ring_buffer.h"
#include "fft.h"

class pitch_detector
{
public:
    pitch_detector(float sample_rate);
    ~pitch_detector();
    
public:
    void set_aref(float aref) { _aref = aref; }
    float get_period(ring_buffer& buffer, float& conf);
    
private:
    float _aref;
    float _conf;
    float _vthresh;
    float _pitch;
    float _sample_rate;
    int _buf_size;
    int _corr_size;
    float _max_period;          // Maximum allowable pitch period (seconds)
    float _min_period;          // Minimum allowable pitch period (seconds)
    int _max_idx;     // Maximum period index for pitch prd est
    int _min_idx;     // Minimum period index for pitch prd est
    
    float *_cbwindow;
    float *_ffttime;
    float *_fftfreqre;
    float *_fftfreqim;
    float *_acwinv;
    
    fft_vars *_fft;
};

#endif