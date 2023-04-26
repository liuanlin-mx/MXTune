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


#ifndef __PITCH_SHIFTER_ST_H__
#define __PITCH_SHIFTER_ST_H__
#include <soundtouch/SoundTouch.h>
#include "pitch_shifter.h"
#include "ring_buffer.h"

class pitch_shifter_st: public pitch_shifter
{
public:
    explicit pitch_shifter_st(std::uint32_t sample_rate);
    ~pitch_shifter_st();
    
public:
    virtual void update_shifter_variables(float inpitch, float outpitch) override;
    virtual void set_aref(float aref) override { _aref = aref; }
    virtual void set_mix(float mix) override {}
    virtual bool set_misc_param(const char *key, const char *value);
    virtual float get_latency() override { return _st.getSetting(SETTING_INITIAL_LATENCY); }
    virtual float shifter(float in) override;
    
private:
    float _aref;
    float _sample_rate;
    soundtouch::SoundTouch _st;
    float _pitch;
};

#endif