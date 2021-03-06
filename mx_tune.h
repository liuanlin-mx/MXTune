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
    enum
    {
        DETECTOR_TYPE_TALENT = 0,
        DETECTOR_TYPE_YIN_FAST = 1,
        DETECTOR_TYPE_YIN = 2,
    };
    
    enum
    {
        SHIFTER_TYPE_TALENT = 0,
        SHIFTER_TYPE_SOUND_TOUCH = 1,
        SHIFTER_TYPE_RUBBERBAND = 2,
    };
    
    
public:
    explicit mx_tune(std::uint32_t sample_rate);
    ~mx_tune();
    
public:
    void set_detector(std::uint32_t detector_type);
    void set_shifter(std::uint32_t shifter_type);
    
    void set_aref(float aref);
    void set_mix(float mix);
    
    void set_at_note(int notes[12]);
    void set_at_pull(float pull);
    void set_at_fixed(float fixed);
    void set_at_shift(float shift);
    void set_at_smooth(float smooth);
    void set_at_amount(float amount);
    void set_at_scwarp(int scwarp);
    
    float get_latency() { return _shifter->get_latency(); }
    float get_inpitch() { return _inpitch; }
    
    float get_conf() { return _conf; }
    float get_conf_shift_thresh() { return _conf_shift_thresh; }
    void set_conf_shift_thresh(float thresh) { _conf_shift_thresh = thresh; }
    void set_conf_detect_thresh(float thresh) { _conf_detect_thresh = thresh; }
    
    void enable_auto_tune(bool b) { _auto_tune = b; }
    manual_tune& get_manual_tune() { return _m_tune; }
    void enable_track(bool b) { _track = b; }
    
    void clear_note();
    void clear_pitch();
    
    void snap_key(float time_min_len, float time_max_interval, float attack, float release, float amount);
    void snap_to_inpitch();
    
    std::list<std::pair<manual_tune::pitch_node, float> > get_outpitch(float time_begin, float time_end);
    
    void run(float *in, float *out, std::int32_t n, float timestamp  = 0.0);
    
private:
    auto_tune _tune;
    manual_tune _m_tune;
    
    std::uint32_t _detector_type;
    std::shared_ptr<pitch_detector> _detector;
    std::uint32_t _shifter_type;
    std::shared_ptr<pitch_shifter> _shifter;
    
    float _aref;
    std::uint32_t _sample_rate;
    std::uint32_t _noverlap;
    float _inpitch;
    float _conf;
    float _conf_detect_thresh;
    float _conf_shift_thresh;
    
    bool _track;
    bool _auto_tune;
    
    std::int32_t _notes[12];

};

#endif