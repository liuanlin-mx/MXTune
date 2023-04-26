/*
 * Copyright (C) 2022-2023 Liu An Lin <liuanlin-mx@qq.com>
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


#ifndef __PITCH_SHIFTER_SMB_H__
#define __PITCH_SHIFTER_SMB_H__
#include <cstdint>
#include "pitch_shifter.h"

class pitch_shifter_smb: public pitch_shifter
{
public:
    explicit pitch_shifter_smb(std::uint32_t sample_rate);
    ~pitch_shifter_smb();
    
public:
    virtual void update_shifter_variables(float inpitch, float outpitch) override;
    virtual void set_aref(float aref) override { _aref = aref; }
    virtual void set_mix(float mix) override {}
    virtual float get_latency() override { return sizeof(_in); }
    virtual float shifter(float in) override;
    
private:
    float _aref;
    float _sample_rate;
    long _osamp;
    long _fft_frame_size;
    float _pitch_shift;
    float _in[512];
    float _out[512];
    std::uint32_t _in_idx;
    std::uint32_t _out_idx;
};

#endif