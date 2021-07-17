#ifndef __PITCH_SHIFTER_ST_H__
#define __PITCH_SHIFTER_ST_H__
#include <memory>
#include <soundtouch/SoundTouch.h>
#include "pitch_shifter.h"
#include "ring_buffer.h"

class pitch_shifter_st: public pitch_shifter
{
public:
    pitch_shifter_st(unsigned int sample_rate);
    ~pitch_shifter_st();
    
public:
    void update_shifter_variables(float inpitch, float outpitch);
    void set_aref(float aref) { _aref = aref; }
    void set_mix(float mix) {}
    float get_latency() { return 0; }
    float shifter(float in);
    
private:
    float _aref;
    float _sample_rate;
    soundtouch::SoundTouch _st;
};

#endif