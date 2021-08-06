#include "mx_tune.h"
#include "pitch_detector_talent.h"
#include "pitch_detector_aubio.h"
#include "pitch_shifter_talent.h"
#include "pitch_shifter_st.h"

mx_tune::mx_tune(unsigned int sample_rate)
    : _tune()
    , _m_tune()
    , _detector_type(DETECTOR_TYPE_YIN_FAST)
    , _detector(new pitch_detector_aubio(sample_rate, "yinfast"))
    , _shifter_type(SHIFTER_TYPE_SOUND_TOUCH)
    , _shifter(new pitch_shifter_st(sample_rate))
    , _aref(440)
    , _sample_rate(sample_rate)
    , _noverlap(4)
    , _inpitch(0)
    , _conf(0.)
    , _conf_detect_thresh(0.7)
    , _conf_shift_thresh(0.9)
    , _track(false)
    , _auto_tune(false)
{
    _detector->set_vthresh(_conf_detect_thresh);
    _detector->set_aref(_aref);
    _shifter->set_aref(_aref);
    _m_tune.set_vthresh(_conf_shift_thresh);
    
    for (std::int32_t i = 0; i < 12; i++)
    {
        _notes[i] = 1;
    }
}

mx_tune::~mx_tune()
{
}

void mx_tune::set_detector(std::uint32_t detector_type)
{
    if (detector_type == _detector_type)
    {
        return;
    }
    
    if (detector_type == DETECTOR_TYPE_TALENT)
    {
        _detector.reset(new pitch_detector_talent(_sample_rate));
        _detector->set_vthresh(_conf_detect_thresh);
        _detector->set_aref(_aref);
        _detector_type = detector_type;
    }
    else if (detector_type == DETECTOR_TYPE_YIN_FAST)
    {
        _detector.reset(new pitch_detector_aubio(_sample_rate, "yinfast"));
        _detector->set_vthresh(_conf_detect_thresh);
        _detector->set_aref(_aref);
        _detector_type = detector_type;
    }
    else if (detector_type == DETECTOR_TYPE_YIN)
    {
        _detector.reset(new pitch_detector_aubio(_sample_rate, "yin"));
        _detector->set_vthresh(_conf_detect_thresh);
        _detector->set_aref(_aref);
        _detector_type = detector_type;
    }
}

void mx_tune::set_shifter(std::uint32_t shifter_type)
{
    if (shifter_type == _shifter_type)
    {
        return;
    }
    
    if (shifter_type == SHIFTER_TYPE_TALENT)
    {
        _shifter.reset(new pitch_shifter_talent(_sample_rate));
        _shifter->set_aref(_aref);
        _shifter_type = shifter_type;
    }
    else if (shifter_type == SHIFTER_TYPE_SOUND_TOUCH)
    {
        _shifter.reset(new pitch_shifter_st(_sample_rate));
        _shifter->set_aref(_aref);
        _shifter_type = shifter_type;
    }
}

void mx_tune::set_aref(float aref)
{
    _aref = aref;
    _detector->set_aref(aref);
    _shifter->set_aref(aref);
}

void mx_tune::set_mix(float mix)
{
    _shifter->set_mix(mix);
}

void mx_tune::set_at_note(int notes[12])
{
    _tune.set_note(notes);
    
    for (std::int32_t i = 0; i < 12; i++)
    {
        _notes[i] = notes[i];
    }
}

void mx_tune::set_at_pull(float pull)
{
    _tune.set_pull(pull);
}

void mx_tune::set_at_fixed(float fixed)
{
    _tune.set_fixed(fixed);
}

void mx_tune::set_at_shift(float shift)
{
    _tune.set_shift(shift);
}

void mx_tune::set_at_smooth(float smooth)
{
    _tune.set_smooth(smooth);
}

void mx_tune::set_at_amount(float amount)
{
    _tune.set_amount(amount);
}

void mx_tune::set_at_scwarp(int scwarp)
{
    _tune.set_scwarp(scwarp);
}

void mx_tune::clear_note()
{
    _m_tune.clear_note();
}

void mx_tune::clear_pitch()
{
    _m_tune.clear_inpitch();
    _m_tune.clear_outpitch();
}

void mx_tune::snap_key(float time_min_len, float time_max_interval, float attack, float release, float amount)
{
    _m_tune.snap_key(_notes, time_min_len, time_max_interval, attack, release, amount);
}

void mx_tune::snap_to_inpitch()
{
    _m_tune.snap_to_inpitch();
}


std::list<std::pair<manual_tune::pitch_node, float> > mx_tune::get_outpitch(float time_begin, float time_end)
{
    auto pitch_list = _m_tune.get_outpitch(time_begin, time_end);
    
    if (_auto_tune)
    {
        for (auto& item : pitch_list)
        {
            if (item.first.conf < _conf_shift_thresh)
            {
                manual_tune::pitch_node inpitch = _m_tune.get_inpitch(item.second);
                if (inpitch.conf >= _conf_shift_thresh)
                {
                    item.first.pitch = _tune.tune(inpitch.pitch);
                    item.first.conf = inpitch.conf;
                }
            }
        }
    }
    return pitch_list;
}


void mx_tune::run(float* in, float *out, std::int32_t n, float timestamp)
{
    for (std::int32_t i = 0; i < n; i++)
    {
        float inpitch;
        float conf;
        
        if (_detector->get_pitch(in[i], inpitch, conf))
        {
            float time_end = timestamp + (float)i / (float)_sample_rate;
            float time_begin = time_end - _detector->get_time();
            float outpitch = inpitch;
            if (_track)
            {
                manual_tune::pitch_node node;
                node.conf = conf;
                node.pitch = inpitch;
                _m_tune.set_inpitch(time_begin, time_end, node);
                
                //node.pitch = _tune.tune(inpitch);
                //_m_tune.set_outpitch(time_begin, time_end, node);
            }
            
            if (conf >= _conf_shift_thresh)
            {
                _inpitch = inpitch;
                _conf = conf;
            
                if (_auto_tune)
                {
                    outpitch = _tune.tune(inpitch);
                }
                
                manual_tune::pitch_node node = _m_tune.get_outpitch(time_begin);
                if (node.conf >= _conf_shift_thresh)
                {
                    outpitch = node.pitch;
                }
            }
            
            _shifter->update_shifter_variables(inpitch, outpitch);
        }
        
        out[i] = _shifter->shifter(in[i]);
    }
}
