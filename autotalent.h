#ifndef __AUTOTALENT2_H__
#define __AUTOTALENT2_H__

#include "ring_buffer.h"
#include "pitch_detector.h"
#include "pitch_shifter.h"
#include "auto_tune.h"

class autotalent
{
public:
    autotalent(unsigned int sample_rate);
    ~autotalent();
    
public:
    void set_aref(float aref);
    void set_mix(float mix);
    void set_note(int notes[12]);
    void update_tune();
    void set_pull(float pull);
    void set_fixed(float fixed);
    void set_shift(float shift);
    void set_smooth(float smooth);
    void set_amount(float amount);
    void set_scwarp(int scwarp);
    float get_latency() { return _buffer.get_buf_size() - 1; }
    void run(float *in, float *out, int n);
    
private:
    auto_tune _tune;
    pitch_detector _detector;
    pitch_shifter _shifter;
    ring_buffer _buffer;
    unsigned int _noverlap;
};

#endif