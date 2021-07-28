#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "pitch_shifter_talent.h"

#define PI (float)3.14159265358979323846

pitch_shifter_talent::pitch_shifter_talent(std::uint32_t sample_rate)
    : pitch_shifter()
    , _aref(440)
    , _sample_rate(sample_rate)
    , _hann_window(NULL)
    , _phasein(0.)
    , _phaseout(0.)
    , _inphinc(0.)
    , _outphinc(0.)
    , _phincfact(0.)
    , _frag(NULL)
    , _fragsize(0)
    , _mix(1.0)
    , _o_buffer(sample_rate)
    , _i_buffer(sample_rate)
{
    std::uint32_t buf_size = _o_buffer.get_buf_size();
    
    _frag = (float *)calloc(buf_size, sizeof(float));
    
    // Standard raised cosine window, max height at N/2
    _hann_window = (float *)calloc(buf_size, sizeof(float));
    for(std::uint32_t i = 0; i < buf_size; i++)
    {
        _hann_window[i] = -0.5 * cos(2 * PI * i / buf_size) + 0.5;
    }
}

pitch_shifter_talent::~pitch_shifter_talent()
{
    free(_frag);
    free(_hann_window);
}

void pitch_shifter_talent::update_shifter_variables(float inpitch, float outpitch)
{
    
    // Compute variables for pitch shifter that depend on pitch
    _inphinc = _aref * pow(2, inpitch / 12) / _sample_rate;
    _outphinc = _aref * pow(2, outpitch / 12) / _sample_rate;
    _phincfact = _outphinc / _inphinc;
    
    //printf("inpitch:%f outpitch:%f phincfact:%f\n", inpitch, outpitch, _phincfact);
}


float pitch_shifter_talent::shifter(float in)
{
    _i_buffer.put(in);
    return _shifter(_i_buffer);
}

    
float pitch_shifter_talent::_shifter(ring_buffer& ibuffer)
{
    int n = ibuffer.get_buf_size();
    // *****************
    // * Pitch Shifter *
    // *****************

    // Pitch shifter (kind of like a pitch-synchronous version of Fairbanks' technique)
    //   Note: pitch estimate is naturally N/2 samples old
    _phasein = _phasein + _inphinc;
    _phaseout = _phaseout + _outphinc;

    // When input phase resets, take a snippet from N/2 samples in the past
    if(_phasein >= 1)
    {
        _phasein = _phasein - 1;
        unsigned int j = ibuffer.get_idx() - n / 2;
        for(int i = -n / 2; i < n / 2; i++)
        {
            _frag[(i + n) % n] = ibuffer[(i + j + n) % n];
        }
    }

    // When output phase resets, put a snippet N/2 samples in the future
    if(_phaseout >= 1)
    {
        _fragsize = _fragsize * 2;
        if(_fragsize > (unsigned int)n)
        {
            _fragsize = n;
        }
           
        _phaseout = _phaseout - 1;
        
        int ti2 = _o_buffer.get_idx() + n / 2;
        int ti3 = (long int)(((float)_fragsize) / _phincfact);
        if(ti3 >= n / 2)
        {
            ti3 = n / 2 - 1;
        }
        
        for(int i = -ti3 / 2; i < (ti3 / 2); i++)
        {
            float tf = _hann_window[(int)n / 2 + i * (int)n / ti3];
            // 3rd degree polynomial interpolator - based on eqns from Hal Chamberlin's book
            int indd = _phincfact * i;
            int ind1 = (int)indd;
            int ind2 = ind1 + 1;
            int ind3 = ind1 + 2;
            int ind0 = ind1 - 1;
            float val0 = _frag[(ind0 + n) % n];
            float val1 = _frag[(ind1 + n) % n];
            float val2 = _frag[(ind2 + n) % n];
            float val3 = _frag[(ind3 + n) % n];
            float vald = 0;
            vald = vald - (float)0.166666666667 * val0 * (indd - ind1) * (indd - ind2) * (indd - ind3);
            vald = vald + (float)0.5 * val1 * (indd - ind0) * (indd - ind2) * (indd - ind3);
            vald = vald - (float)0.5 * val2 * (indd - ind0) * (indd - ind1) * (indd - ind3);
            vald = vald + (float)0.166666666667 * val3 * (indd - ind0) * (indd - ind1) * (indd - ind2);
            _o_buffer[(i + ti2 + n) % n] = _o_buffer[(i + ti2 + n) % n] + vald * tf;
        }
        _fragsize = 0;
    }
    
    _fragsize++;

    //   Get output signal from buffer
    float tf = _o_buffer.current(); // read buffer

    _o_buffer.put(0); // erase for next cycle

    // *********************
    // * END Pitch Shifter *
    // *********************

    unsigned int ti4 = (ibuffer.get_idx() + 2) % n;

    // Write audio to output of plugin
    // Mix (blend between original (delayed) =0 and processed =1)
    return (float)_mix * tf + (1 - _mix) * ibuffer[ti4];
}
