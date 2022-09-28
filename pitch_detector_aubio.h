#ifndef __PITCH_DETECTOR_AUBIO_H__
#define __PITCH_DETECTOR_AUBIO_H__
#include <fftw3.h>
#include <aubio/aubio.h>
#include <cstdint>
#include <assert.h>
#include "SvfLinearTrapOptimised2.hpp"
#include "pitch_detector.h"

class pitch_detector_aubio: public pitch_detector
{
public:
    pitch_detector_aubio(float sample_rate, const char *method = "yin");
    ~pitch_detector_aubio();
    
private:
    pitch_detector_aubio(const pitch_detector_aubio&);
    pitch_detector_aubio& operator=(const pitch_detector_aubio&);
    
public:
    virtual void set_vthresh(float vthresh) override { _vthresh = vthresh; }
    virtual void set_gate(float db) override {}
    virtual void set_aref(float aref) override { _aref = aref; }
    virtual bool get_pitch(float in, float& pitch, float& conf) override;
    virtual float get_time() override { return (float)_hop_s * 2 / _sample_rate; }
    
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
    SvfLinearTrapOptimised2 _svf_filter_lp;
    SvfLinearTrapOptimised2 _svf_filter_hp;
};

#endif