#ifndef __PITCH_DETECTOR_H__
#define __PITCH_DETECTOR_H__
#include <cstdint>

class pitch_detector
{
public:
    pitch_detector() {}
    virtual ~pitch_detector() {};
    
public:
    virtual void set_vthresh(float vthresh) = 0;
    virtual void set_aref(float aref) = 0;
    virtual bool get_pitch(float in, float& pitch, float& conf) = 0;
    virtual float get_time() = 0;
};

#endif