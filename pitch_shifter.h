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


#ifndef __PITCH_SHIFTER_H__
#define __PITCH_SHIFTER_H__
#include "ring_buffer.h"

class pitch_shifter
{
public:
    pitch_shifter() {}
    virtual ~pitch_shifter() {}
    
public:
    virtual void update_shifter_variables(float inpitch, float outpitch) = 0;
    virtual void set_aref(float aref) = 0;
    virtual void set_mix(float mix) = 0;
    virtual bool set_misc_param(const char *key, const char *value) { return false; }
    virtual float get_latency() = 0;
    virtual float shifter(float in) = 0;
};

#endif