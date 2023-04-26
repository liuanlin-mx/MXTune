/*
 * Copyright (C) 2021-2023 Liu An Lin <liuanlin-mx@qq.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


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