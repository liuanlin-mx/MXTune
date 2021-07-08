#ifndef __AUTOTALENT2_H__
#define __AUTOTALENT2_H__

#include "ring_buffer.h"
#include "pitch_detector.h"
#include "pitch_shifter.h"
#include "auto_tune.h"
#include "manual_tune.h"

class mx_tune
{
public:
    mx_tune(std::uint32_t sample_rate);
    ~mx_tune();
    
public:
    void set_aref(float aref);
    void set_mix(float mix);
    void set_at_note(int notes[12]);
    void set_at_pull(float pull);
    void set_at_fixed(float fixed);
    void set_at_shift(float shift);
    void set_at_smooth(float smooth);
    void set_at_amount(float amount);
    void set_at_scwarp(int scwarp);
    float get_latency() { return _shifter.get_latency(); }
    float get_inpitch() { return _inpitch; }
    float get_conf() { return _conf; }
    float get_conf_thresh() { return _conf_thresh; }
    
    void enable_auto_tune(bool b) { _auto_tune = b; }
    manual_tune& get_manual_tune() { return _m_tune; }
    void enable_track(bool b) { _track = b; }
    void clear_note();
    void clear_pitch();
    void snap_key();
    void snap_to_inpitch();
    
    
    void run(float *in, float *out, std::int32_t n, float timestamp  = 0.0);
    
private:
    auto_tune _tune;
    manual_tune _m_tune;
    
    pitch_detector _detector;
    pitch_shifter _shifter;
    
    std::uint32_t _sample_rate;
    std::uint32_t _noverlap;
    float _inpitch;
    float _conf;
    float _conf_thresh;
    
    volatile bool _track;
    volatile bool _auto_tune;

};

#endif