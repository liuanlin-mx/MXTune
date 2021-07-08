#ifndef __PITCH_SHIFTER_H__
#define __PITCH_SHIFTER_H__
#include "ring_buffer.h"

class pitch_shifter
{
public:
    pitch_shifter(unsigned int sample_rate);
    ~pitch_shifter();
    
public:
    void update_shifter_variables(float inpitch, float outpitch);
    void set_aref(float aref) { _aref = aref; }
    void set_mix(float mix) {_mix = mix;}
    float get_latency() { return _o_buffer.get_buf_size() - 1; }
    float shifter(float in);
    
private:
    float _shifter(ring_buffer& ibuffer);
    
private:
    float _aref;
    float _sample_rate;
    
    float *_hann_window;
    
    float _phasein;
    float _phaseout;
    float _inphinc;
    float _outphinc;
    float _phincfact;
    
    float *_frag;
    unsigned int _fragsize;
    
    float _mix;
    ring_buffer _o_buffer;
    
    float _test_in[32 * 1024];
    float _test_out[32 * 1024];
    int _test_idx;
    
    ring_buffer _i_buffer;
};

#endif