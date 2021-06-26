#include "mx_tune.h"
#include "net_log.h"

mx_tune::mx_tune(unsigned int sample_rate)
    : _detector(sample_rate)
    , _shifter(sample_rate)
    , _buffer(sample_rate)
    , _sample_rate(sample_rate)
    , _noverlap(4)
    , _inpitch(0)
    , _conf(0.)
    , _conf_thresh(0.7)
    , _track(false)
    , _auto_tune(false)
{
    _detector.set_vthresh(_conf_thresh);
    _m_tune.set_vthresh(_conf_thresh);
}

mx_tune::~mx_tune()
{
}

void mx_tune::set_aref(float aref)
{
    _detector.set_aref(aref);
    _shifter.set_aref(aref);
}

void mx_tune::set_mix(float mix)
{
    _shifter.set_mix(mix);
}

void mx_tune::set_at_note(int notes[12])
{
    _tune.set_note(notes);
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

void mx_tune::snap_key()
{
    _m_tune.snap_key(&_tune);
}

void mx_tune::run(float* in, float *out, int n, float timestamp)
{
    for (int i = 0; i < n; i++)
    {
        _buffer.put(in[i]);
        if(_buffer.get_idx() % (_buffer.get_buf_size() / _noverlap) == 0)
        {
            float time_begin = timestamp + (float)i / (float)_sample_rate;
            float time_end = timestamp + (float)(i + _buffer.get_buf_size() / _noverlap) / (float)_sample_rate;
            
            float inpitch = _detector.get_period(_buffer, _conf);
            float outpitch = inpitch;
            _inpitch = inpitch;
            
            if (_track)
            {
                manual_tune::pitch_node node;
                node.conf = _conf;
                node.pitch = inpitch;
                //net_log_debug("track conf:%f pitch:%f time:%f %f\n", node.conf, node.pitch, time_begin, time_end);
                _m_tune.set_inpitch(time_begin, time_end, node);
                
                //node.pitch = _tune.tune(inpitch);
                //_m_tune.set_outpitch(time_begin, time_end, node);
            }
            
            if (_auto_tune)
            {
                outpitch = _tune.tune(inpitch);
            }
            
            
            manual_tune::pitch_node node = _m_tune.get_outpitch(time_begin);
            if (node.conf > 0.)
            {
                outpitch = node.pitch;
            }
            
            _shifter.update_shifter_variables(inpitch, outpitch);
            
        }
        out[i] = _shifter.shifter(_buffer);
    }
}
