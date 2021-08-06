#include <stdio.h>
#include <malloc.h>
#include <math.h>

#include "pitch_shifter_rb.h"


pitch_shifter_rb::pitch_shifter_rb(std::uint32_t sample_rate)
    : _aref(440)
    , _sample_rate(sample_rate)
    , _rb(sample_rate, 1,
        RubberBand::RubberBandStretcher::OptionProcessRealTime
        | RubberBand::RubberBandStretcher::OptionTransientsSmooth
        | RubberBand::RubberBandStretcher::OptionPitchHighConsistency
        | RubberBand::RubberBandStretcher::OptionWindowShort
        | RubberBand::RubberBandStretcher::OptionDetectorPercussive
        | RubberBand::RubberBandStretcher::OptionThreadingAuto)
{
    _rb.setPitchScale(1.0);
}

pitch_shifter_rb::~pitch_shifter_rb()
{
}

void pitch_shifter_rb::update_shifter_variables(float inpitch, float outpitch)
{
    float scale;
    if (inpitch > outpitch)
    {
        scale = 1.0 - (inpitch - outpitch) / 12.;
    }
    else
    {
        scale = 1.0 + (outpitch - inpitch) / 12.;
    }
    
    if (scale < 3.0 && scale > 0.33)
    {
        _rb.setPitchScale(scale);
    }
}


float pitch_shifter_rb::shifter(float in)
{
    float ibs[1] = {in};
    float *ibuf[1] = {ibs};
    
    float obs[1];
    float *obuf[1] = {obs};
    
    _rb.process(ibuf, 1, false);
    if (_rb.retrieve(obuf, 1) > 0)
    {
        return obs[0];
    }
    return 0.;
}
