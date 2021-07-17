#include <math.h>
#include <string.h>
#include "pitch_detector_aubio.h"

#define PI (float)3.14159265358979323846
#define L2SC (float)3.32192809488736218171

pitch_detector_aubio::pitch_detector_aubio(float sample_rate, const char *method)
    : _sample_rate(sample_rate)
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
}


pitch_detector_aubio::~pitch_detector_aubio()
{
    del_aubio_pitch(_pitch_detector);
    del_fvec(_out);
    del_fvec(_input);
}


bool pitch_detector_aubio::get_pitch(float in, float& pitch, float& conf)
{
    fvec_set_sample(_input, in, _ilen);
    if (++_ilen >= _hop_s)
    {
        _ilen = 0;
        aubio_pitch_do(_pitch_detector, _input, _out);
        
        conf = aubio_pitch_get_confidence(_pitch_detector);
        float freq = fvec_get_sample(_out, 0);
        if (conf > _vthresh)
        {
            _pitch = (float)12. * log10(freq / (float)_aref) * L2SC;
        }
        pitch = _pitch;
        return true;
    }
    return false;
}
