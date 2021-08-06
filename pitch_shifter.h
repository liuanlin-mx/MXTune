#ifndef __PITCH_SHIFTER_H__
#define __PITCH_SHIFTER_H__
#include "ring_buffer.h"

class pitch_shifter
{
public:
    pitch_shifter() {}
    virtual ~pitch_shifter() {}
    
public:
    virtual void update_shifter_variables(float inpitch, float outpitch) = 0;
    virtual void set_aref(float aref) = 0;
    virtual void set_mix(float mix) = 0;
    virtual float get_latency() = 0;
    virtual float shifter(float in) = 0;
};

#endif