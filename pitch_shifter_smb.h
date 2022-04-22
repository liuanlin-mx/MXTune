#ifndef __PITCH_SHIFTER_SMB_H__
#define __PITCH_SHIFTER_SMB_H__
#include <cstdint>
#include "pitch_shifter.h"

class pitch_shifter_smb: public pitch_shifter
{
public:
    explicit pitch_shifter_smb(std::uint32_t sample_rate);
    ~pitch_shifter_smb();
    
public:
    virtual void update_shifter_variables(float inpitch, float outpitch) override;
    virtual void set_aref(float aref) override { _aref = aref; }
    virtual void set_mix(float mix) override {}
    virtual float get_latency() override { return sizeof(_in); }
    virtual float shifter(float in) override;
    
private:
    float _aref;
    float _sample_rate;
    long _osamp;
    long _fft_frame_size;
    float _pitch_shift;
    float _in[512];
    float _out[512];
    std::uint32_t _in_idx;
    std::uint32_t _out_idx;
};

#endif