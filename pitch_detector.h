#ifndef __PITCH_DETECTOR_H__
#define __PITCH_DETECTOR_H__
#include "ring_buffer.h"
#include <fftw3.h>
#include <cstdint>

class pitch_detector
{
public:
    pitch_detector(float sample_rate);
    ~pitch_detector();
    
public:

    void set_vthresh(float vthresh) { _vthresh = vthresh; }
    void set_aref(float aref) { _aref = aref; }
    bool get_period(float in, float& pitch, float& conf);
    float get_time() { return (_buffer.get_buf_size() / _noverlap) / _sample_rate; }
    
private:
    float _get_period(ring_buffer& buffer, float& conf);
    
private:
    float _aref;
    float _conf;
    float _vthresh;
    float _pitch;
    float _sample_rate;
    std::int32_t _buf_size;
    std::int32_t _corr_size;
    float _max_period;          // Maximum allowable pitch period (seconds)
    float _min_period;          // Minimum allowable pitch period (seconds)
    std::int32_t _max_idx;     // Maximum period index for pitch prd est
    std::int32_t _min_idx;     // Minimum period index for pitch prd est
    
    float *_cbwindow;
    float *_ffttime;
    float *_acwinv;
    fftwf_complex* _complex;
    fftwf_plan _forward_plan;
    fftwf_plan _reverse_plan;
    
    ring_buffer _buffer;
    std::int32_t _noverlap;
};

#endif