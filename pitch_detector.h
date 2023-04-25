#ifndef __PITCH_DETECTOR_H__
#define __PITCH_DETECTOR_H__
#include <cstdint>

class pitch_detector
{
protected:
    enum
    {
        MIN_PITCH = -48, //27.5Hz
        MAX_PITCH = 39, //4186Hz
    };
    
public:
    pitch_detector() {}
    virtual ~pitch_detector() {};
    
public:
    virtual void set_vthresh(float vthresh) = 0;
    virtual void set_gate(float db) = 0;
    virtual void set_aref(float aref) = 0;
    virtual void set_freq_range(float min_freq, float max_freq) = 0;
    virtual bool set_misc_param(const char *key, const char *value) { return false; }
    virtual bool get_pitch(float in, float& pitch, float& conf) = 0;
    virtual float get_time() = 0;
};

#endif