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


#include <stdio.h>
#include <malloc.h>
#include <math.h>

#include "pitch_shifter_smb.h"

void smbPitchShift(float pitchShift, long numSampsToProcess, long fftFrameSize, long osamp, float sampleRate, float *indata, float *outdata);


pitch_shifter_smb::pitch_shifter_smb(std::uint32_t sample_rate)
    : _aref(440)
    , _sample_rate(sample_rate)
    , _osamp(32)
    , _fft_frame_size(sizeof(_in) / sizeof(_in[0]))
    , _pitch_shift(1)
    , _in_idx(0)
    , _out_idx(0)
{
}

pitch_shifter_smb::~pitch_shifter_smb()
{
}

void pitch_shifter_smb::update_shifter_variables(float inpitch, float outpitch)
{
    float pitch_diff = (outpitch - inpitch);
    if (pitch_diff > -12 && pitch_diff < 12)
    {
        _pitch_shift = pow(2., pitch_diff / 12.);
    }
    
    if (_pitch_shift < 0.5)
    {
        _pitch_shift = 0.5;
    }
    if (_pitch_shift > 2)
    {
        _pitch_shift = 2;
    }
}


float pitch_shifter_smb::shifter(float in)
{
    _in[_in_idx] = in;
    _in_idx++;
    if (_in_idx >= sizeof(_in) / sizeof(_in[0]))
    {
        smbPitchShift(_pitch_shift, sizeof(_in) / sizeof(_in[0]), _fft_frame_size, _osamp, _sample_rate, _in, _out);
        _in_idx = 0;
        _out_idx = 0;
    }
    
    if (_out_idx < sizeof(_out) / sizeof(_out[0]))
    {
        std::uint32_t out_idx = _out_idx;
        if (++_out_idx >= sizeof(_out) / sizeof(_out[0]))
        {
            _out_idx = 0;
        }
        return _out[out_idx];
    }
    return 0;
}
