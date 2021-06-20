#ifndef __AUTOTALENT2_H__
#define __AUTOTALENT2_H__

#include "ring_buffer.h"
#include "pitch_detector.h"
#include "pitch_shifter.h"
#include "auto_tune.h"
#include "manual_tune.h"

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
    float get_inpitch() { return _inpitch; }
    float get_conf() { return _conf; }
    float get_conf_thresh() { return _conf_thresh; }
    
    void enable_auto_tune(bool b) { _auto_tune = b; }
    manual_tune& get_manual_tune() { return _m_tune; }
    void enable_track(bool b) { _track = b; }
    void clear_note();
    void clear_pitch();
    void snap_key();
    
    void run(float *in, float *out, int n, float timestamp  = 0.0);
    
private:
    auto_tune _tune;
    manual_tune _m_tune;
    
    pitch_detector _detector;
    pitch_shifter _shifter;
    ring_buffer _buffer;
    unsigned int _sample_rate;
    unsigned int _noverlap;
    float _inpitch;
    float _conf;
    float _conf_thresh;
    
    volatile bool _track;
    volatile bool _auto_tune;

};

#endif