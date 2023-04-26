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


#ifndef __PITCH_DETECTOR_TALENT_H__
#define __PITCH_DETECTOR_TALENT_H__
#include <fftw3.h>
#include <cstdint>
#include "ring_buffer.h"
#include "pitch_detector.h"

class pitch_detector_talent: public pitch_detector
{
public:
    explicit pitch_detector_talent(float sample_rate);
    ~pitch_detector_talent();
    
private:
    pitch_detector_talent(const pitch_detector_talent&);
    pitch_detector_talent& operator=(const pitch_detector_talent&);
    
public:
    virtual void set_vthresh(float vthresh) override { _vthresh = vthresh; }
    virtual void set_gate(float db) override { _gate = db; }
    virtual void set_aref(float aref) override { _aref = aref; }
    virtual void set_freq_range(float min_freq, float max_freq);
    virtual bool get_pitch(float in, float& pitch, float& conf) override;
    virtual float get_time() override { return (_buffer.get_buf_size() / _noverlap) / _sample_rate; }
    
private:
    float _get_pitch(ring_buffer& buffer, float& conf);
    
private:
    float _aref;
    float _conf;
    float _vthresh;
    float _gate;
    float _pitch;
    float _sample_rate;
    std::int32_t _buf_size;
    std::int32_t _corr_size;
    float _max_period;          // Maximum allowable pitch period (seconds)
    float _min_period;          // Minimum allowable pitch period (seconds)
    std::int32_t _max_idx;     // Maximum period index for pitch prd est
    std::int32_t _min_idx;     // Minimum period index for pitch prd est
    
    float *_cbwindow;
    float *_ffttime;
    float *_acwinv;
    fftwf_complex* _complex;
    fftwf_plan _forward_plan;
    fftwf_plan _reverse_plan;
    
    ring_buffer _buffer;
    std::int32_t _noverlap;
};

#endif