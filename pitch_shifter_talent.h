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


#ifndef __PITCH_SHIFTER_TALENT_H__
#define __PITCH_SHIFTER_TALENT_H__
#include <cstdint>
#include "pitch_shifter.h"
#include "ring_buffer.h"

class pitch_shifter_talent: public pitch_shifter
{
public:
    explicit pitch_shifter_talent(std::uint32_t sample_rate);
    ~pitch_shifter_talent();
    
private:
    pitch_shifter_talent(const pitch_shifter_talent&);
    pitch_shifter_talent& operator=(const pitch_shifter_talent&);
    
public:
    virtual void update_shifter_variables(float inpitch, float outpitch) override;
    virtual void set_aref(float aref) override { _aref = aref; }
    virtual void set_mix(float mix) override {_mix = mix;}
    virtual float get_latency() override { return _o_buffer.get_buf_size() - 1; }
    virtual float shifter(float in) override;
    
private:
    float _shifter(ring_buffer& ibuffer);
    
private:
    float _aref;
    float _sample_rate;
    
    float *_hann_window;
    
    float _phasein;
    float _phaseout;
    float _inphinc;
    float _outphinc;
    float _phincfact;
    
    float *_frag;
    std::uint32_t _fragsize;
    
    float _mix;
    
    ring_buffer _o_buffer;
    ring_buffer _i_buffer;
};

#endif