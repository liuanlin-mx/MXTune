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
    
    _st.setSetting(SETTING_SEQUENCE_MS, 40);
    _st.setSetting(SETTING_SEEKWINDOW_MS, 15);
    _st.setSetting(SETTING_OVERLAP_MS, 8);
}

pitch_shifter_st::~pitch_shifter_st()
{
}

void pitch_shifter_st::update_shifter_variables(float inpitch, float outpitch)
{
    float pitch_diff = (outpitch - inpitch);
    if (pitch_diff > -12 && pitch_diff < 12)
    {
        _st.setPitchSemiTones(pitch_diff);
    }
}


float pitch_shifter_st::shifter(float in)
{
    soundtouch::SAMPLETYPE input[1] = {in};
    _st.putSamples(input, 1);
    
    soundtouch::SAMPLETYPE output[1] = {0.};
    _st.receiveSamples(output, 1);
    return output[0];
}
