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


#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include "pitch_detector_aubio.h"

#define PI (float)3.14159265358979323846
#define L2SC (float)3.32192809488736218171

pitch_detector_aubio::pitch_detector_aubio(float sample_rate, const char *method)
    : pitch_detector()
    , _sample_rate(sample_rate)
    , _aref(440)
    , _vthresh(0.9)
    , _pitch(0)
    , _win_s(2048)
    , _hop_s(_win_s / 4)
    , _input(new_fvec(_hop_s))
    , _ilen(0)
    , _out(new_fvec(1))
    , _pitch_detector(new_aubio_pitch(method, _win_s, _hop_s, _sample_rate))
{
    aubio_pitch_set_unit(_pitch_detector, "Hz");
    aubio_pitch_set_silence(_pitch_detector, -200);
    //aubio_pitch_set_tolerance(_pitch_detector, 0.8);
    _svf_filter_lp.updateCoefficients(800, 0.5, SvfLinearTrapOptimised2::LOW_PASS_FILTER, _sample_rate);
    _svf_filter_hp.updateCoefficients(70, 0.5, SvfLinearTrapOptimised2::HIGH_PASS_FILTER, _sample_rate);
}


pitch_detector_aubio::~pitch_detector_aubio()
{
    del_aubio_pitch(_pitch_detector);
    del_fvec(_out);
    del_fvec(_input);
}


void pitch_detector_aubio::set_gate(float db)
{
    aubio_pitch_set_silence(_pitch_detector, db);
}

void pitch_detector_aubio::set_freq_range(float min_freq, float max_freq)
{
    _svf_filter_lp.resetState();
    _svf_filter_lp.updateCoefficients(max_freq, 0.5, SvfLinearTrapOptimised2::LOW_PASS_FILTER, _sample_rate);
    _svf_filter_hp.resetState();
    _svf_filter_hp.updateCoefficients(min_freq, 0.5, SvfLinearTrapOptimised2::HIGH_PASS_FILTER, _sample_rate);
}
    
bool pitch_detector_aubio::get_pitch(float in, float& pitch, float& conf)
{
    in = _svf_filter_lp.tick(_svf_filter_hp.tick(in));
    fvec_set_sample(_input, in, _ilen);
    if (++_ilen >= _hop_s)
    {
        _ilen = 0;
        aubio_pitch_do(_pitch_detector, _input, _out);
        
        conf = aubio_pitch_get_confidence(_pitch_detector);
        float freq = fvec_get_sample(_out, 0);
        if (conf > _vthresh)
        {
            float pitch_ = (float)12. * log10(freq / (float)_aref) * L2SC;
            if (pitch_ < MIN_PITCH || pitch_ > MAX_PITCH)
            {
                conf = 0.;
            }
            else
            {
                _pitch = pitch_;
            }
        }
        pitch = _pitch;
        return true;
    }
    return false;
}
