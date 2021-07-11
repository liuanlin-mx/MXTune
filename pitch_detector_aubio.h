#ifndef __PITCH_DETECTOR_AUBIO_H__
#define __PITCH_DETECTOR_AUBIO_H__
#include <fftw3.h>
#include <aubio/aubio.h>
#include <cstdint>
#include "pitch_detector.h"

class pitch_detector_aubio: public pitch_detector
{
public:
    pitch_detector_aubio(float sample_rate, const char *method = "yin");
    ~pitch_detector_aubio();
    
public:
    void set_vthresh(float vthresh) { _vthresh = vthresh; }
    void set_aref(float aref) { _aref = aref; }
    bool get_pitch(float in, float& pitch, float& conf);
    float get_time() { return (float)_hop_s * 2 / _sample_rate; }
    
private:
    float _sample_rate;
    float _aref;
    float _vthresh;
    float _pitch;
    
    uint_t _win_s; // window size
    uint_t _hop_s; // hop size
    
    // create some vectors
    fvec_t *_input; // input buffer
    std::uint32_t _ilen;
    fvec_t *_out; // output candidates
    
    aubio_pitch_t *_pitch_detector;
};

#endif