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


#ifndef __PITCH_DETECTOR_AUBIO_H__
#define __PITCH_DETECTOR_AUBIO_H__
#define _USE_MATH_DEFINES
#include <math.h>
#include <fftw3.h>
#include <aubio/aubio.h>
#include <cstdint>
#include <assert.h>
#include "SvfLinearTrapOptimised2.hpp"
#include "pitch_detector.h"

class pitch_detector_aubio: public pitch_detector
{
public:
    pitch_detector_aubio(float sample_rate, const char *method = "yin");
    ~pitch_detector_aubio();
    
private:
    pitch_detector_aubio(const pitch_detector_aubio&);
    pitch_detector_aubio& operator=(const pitch_detector_aubio&);
    
public:
    virtual void set_vthresh(float vthresh) override { _vthresh = vthresh; }
    virtual void set_gate(float db);
    virtual void set_aref(float aref) override { _aref = aref; }
    virtual void set_freq_range(float min_freq, float max_freq);
    virtual bool get_pitch(float in, float& pitch, float& conf) override;
    virtual float get_time() override { return (float)_hop_s * 2 / _sample_rate; }
    
private:
    float _sample_rate;
    float _aref;
    float _vthresh;
    float _pitch;
    
    uint_t _win_s; // window size
    uint_t _hop_s; // hop size
    
    // create some vectors
    fvec_t *_input; // input buffer
    std::uint32_t _ilen;
    fvec_t *_out; // output candidates
    
    aubio_pitch_t *_pitch_detector;
    SvfLinearTrapOptimised2 _svf_filter_lp;
    SvfLinearTrapOptimised2 _svf_filter_hp;
};

#endif