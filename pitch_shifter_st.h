#ifndef __PITCH_SHIFTER_ST_H__
#define __PITCH_SHIFTER_ST_H__
#include <soundtouch/SoundTouch.h>
#include "pitch_shifter.h"
#include "ring_buffer.h"

class pitch_shifter_st: public pitch_shifter
{
public:
    explicit pitch_shifter_st(unsigned int sample_rate);
    ~pitch_shifter_st();
    
public:
    virtual void update_shifter_variables(float inpitch, float outpitch) override;
    virtual void set_aref(float aref) override { _aref = aref; }
    virtual void set_mix(float mix) override {}
    virtual float get_latency() override { return _st.getSetting(SETTING_INITIAL_LATENCY); }
    virtual float shifter(float in) override;
    
private:
    float _aref;
    float _sample_rate;
    soundtouch::SoundTouch _st;
};

#endif