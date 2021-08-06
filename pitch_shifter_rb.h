#ifndef __PITCH_SHIFTER_RB_H__
#define __PITCH_SHIFTER_RB_H__
#include <cstdint>
#include <rubberband/RubberBandStretcher.h>
#include "pitch_shifter.h"
#include "ring_buffer.h"

class pitch_shifter_rb: public pitch_shifter
{
public:
    explicit pitch_shifter_rb(std::uint32_t sample_rate);
    ~pitch_shifter_rb();
    
public:
    virtual void update_shifter_variables(float inpitch, float outpitch) override;
    virtual void set_aref(float aref) override { _aref = aref; }
    virtual void set_mix(float mix) override {}
    virtual float get_latency() override { return _rb.getLatency(); }
    virtual float shifter(float in) override;
    
private:
    float _aref;
    float _sample_rate;
    RubberBand::RubberBandStretcher _rb;
};

#endif