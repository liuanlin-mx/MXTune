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


#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "pitch_shifter_st.h"


pitch_shifter_st::pitch_shifter_st(std::uint32_t sample_rate)
    : _aref(440)
    , _sample_rate(sample_rate)
    , _st()
{
    _st.setSampleRate(sample_rate);
    _st.setChannels(1);
    
    _st.setPitchSemiTones(0.);
    
    _st.setSetting(SETTING_USE_QUICKSEEK, 0);
    _st.setSetting(SETTING_USE_AA_FILTER, 1);
    
    //_st.setSetting(SETTING_SEQUENCE_MS, 12);
    _st.setSetting(SETTING_SEQUENCE_MS, 16);
    _st.setSetting(SETTING_SEEKWINDOW_MS, 4);
    _st.setSetting(SETTING_OVERLAP_MS, 4);
}

pitch_shifter_st::~pitch_shifter_st()
{
}

void pitch_shifter_st::update_shifter_variables(float inpitch, float outpitch)
{
    float pitch_diff = (outpitch - inpitch);
    if (pitch_diff > -12 && pitch_diff < 12)
    {
        _pitch = pitch_diff;
        _st.setPitchSemiTones(pitch_diff);
    }
    else
    {
        _st.setPitchSemiTones(0);
    }
}

bool pitch_shifter_st::set_misc_param(const char *key, const char *value)
{
    if (std::string(key) == "st.sequence_ms")
    {
        std::int32_t v = atoi(value);
        if (v > 0 && v < 500)
        {
            _st.setSetting(SETTING_SEQUENCE_MS, v);
            return true;
        }
    }
    else if (std::string(key) == "st.seekwindow_ms")
    {
        std::int32_t v = atoi(value);
        if (v > 0 && v < 500)
        {
            _st.setSetting(SETTING_SEEKWINDOW_MS, v);
            return true;
        }
    }
    else if (std::string(key) == "st.overlap_ms")
    {
        std::int32_t v = atoi(value);
        if (v > 0 && v < 500)
        {
            _st.setSetting(SETTING_OVERLAP_MS, v);
            return true;
        }
    }
    return false;
}

float pitch_shifter_st::shifter(float in)
{
    soundtouch::SAMPLETYPE input[1] = {in};
    _st.putSamples(input, 1);
    
    soundtouch::SAMPLETYPE output[1] = {0.};
    uint r = _st.receiveSamples(output, 1);
    
    return output[0];
}
