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


#include "pitch_detector_aubio.h"
#include "pitch_detector_talent.h"
#include "pitch_shifter_talent.h"
#include "pitch_shifter_st.h"
#include "pitch_shifter_rb.h"
#include "pitch_shifter_smb.h"
#include "mx_tune.h"

text_readline::text_readline()
    : _text(NULL)
    , _len(0)
    , _cur(NULL)
{
}

text_readline::~text_readline()
{
}



void text_readline::load(const char *text, std::uint32_t len)
{
    _text = text;
    _len = len;
    _cur = _text;
}


std::string text_readline::read_line()
{
    const char *end = _text + _len;
    _cur = _skip_space(_cur, end);
    const char *str = _cur;
    
    while (_cur < end)
    {
        if (*_cur == '\r' || *_cur == '\n')
        {
            while (*_cur == '\r' || *_cur == '\n')
            {
                _cur++;
            }
            break;
        }
        _cur++;
    }
    return std::string(str, _cur - str);
}



std::string text_readline::look_line()
{
    const char *end = _text + _len;
    const char *cur = _skip_space(_cur, end);
    const char *str = cur;
    
    while (cur < end)
    {
        if (*cur == '\r' || *cur == '\n')
        {
            while (*cur == '\r' || *cur == '\n')
            {
                cur++;
            }
            break;
        }
        cur++;
    }
    return std::string(str, cur - str);
}


const char *text_readline::_skip_space(const char *str, const char *end)
{
    while (str < end)
    {
        if (!std::isspace(*str))
        {
            break;
        }
        str++;
    }
    return str;
}


mx_tune::mx_tune(unsigned int sample_rate)
    : _tune()
    , _m_tune()
    , _detector_type(DETECTOR_TYPE_YIN_FAST)
    , _detector(new pitch_detector_aubio(sample_rate, "yinfast"))
    , _shifter_type(SHIFTER_TYPE_SOUND_TOUCH)
    , _shifter(new pitch_shifter_st(sample_rate))
    , _delay(sample_rate)
    , _aref(440)
    , _misc()
    , _sample_rate(sample_rate)
    , _noverlap(4)
    , _inpitch(0)
    , _conf(0.)
    , _conf_detect_thresh(0.7)
    , _conf_shift_thresh(0.9)
    , _det_min_freq(70)
    , _det_max_freq(800)
    , _det_gate(-60)
    , _track(false)
    , _auto_tune(false)
    , _midi_note_on(false)
    , _midi_note_off(false)
    , _midi_note_on_time(0)
    , _midi_note(0)
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
    _set_detector(detector_type);
}

void mx_tune::set_shifter(std::uint32_t shifter_type)
{
    if (shifter_type == _shifter_type)
    {
        return;
    }
    _set_shifter(shifter_type);
}

void mx_tune::set_sample_rate(std::uint32_t sample_rate)
{
    if (_sample_rate != sample_rate)
    {
        _sample_rate = sample_rate;
        _set_detector(_detector_type);
        _set_shifter(_shifter_type);
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

void mx_tune::set_misc_param(const std::string& misc)
{
    _misc = misc;
    _apply_misc_param();
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

void mx_tune::set_conf_shift_thresh(float thresh)
{
    _conf_shift_thresh = thresh;
    _m_tune.set_vthresh(_conf_shift_thresh);
}

void mx_tune::set_conf_detect_thresh(float thresh)
{
    _conf_detect_thresh = thresh;
    _detector->set_vthresh(_conf_detect_thresh);
}

void mx_tune::set_detect_freq_range(float min_freq, float max_freq)
{
    _det_min_freq = min_freq;
    _det_max_freq = max_freq;
    _detector->set_freq_range(_det_min_freq, _det_max_freq);
}

void mx_tune::set_detect_gate(float db)
{
    _det_gate = db;
    _detector->set_gate(_det_gate);
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

void mx_tune::enable_history()
{
    _m_tune.enable_history();
}

void mx_tune::disable_history()
{
    _m_tune.disable_history();
}

void mx_tune::undo()
{
    _m_tune.undo();
}

void mx_tune::redo()
{
    _m_tune.redo();
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
        
        out[i] = _delay.process(_shifter->shifter(in[i]));
    }
}


void mx_tune::record_midi_to_note(std::int32_t n, float timestamp,
                                const std::list<midi_msg_node>& midi_list,
                                float attack, float release, float amount)
{
    if (!_track)
    {
        return;
    }
    
    float midi_note_off_time = 0;
    for (auto& msg: midi_list)
    {
        if (msg.msg.is_note_on())
        {
            if (_midi_note_on == false)
            {
                _midi_note_on = true;
                _midi_note_off = false;
                _midi_note = msg.msg.get_note();
                _midi_note_on_time = timestamp + (float)msg.sample_position / (float)_sample_rate;
            }
            else
            {
                midi_note_off_time = timestamp + (float)msg.sample_position / (float)_sample_rate;
                if (_midi_note_on_time > midi_note_off_time)
                {
                    _midi_note_on = false;
                    _midi_note_off = false;
                    _midi_note_on_time = 0;
                }
                else
                {
                    std::shared_ptr<manual_tune::tune_node> tune(new manual_tune::tune_node());
                    tune->time_start = _midi_note_on_time;
                    tune->time_end = midi_note_off_time;
                    tune->pitch_start = _midi_note - 69;
                    tune->pitch_end = _midi_note - 69;
                    tune->attack = attack;
                    tune->release = release;
                    tune->amount = amount;
                    _m_tune.disable_history();
                    _m_tune.add_tune(tune);
                    _m_tune.enable_history();
                    
                    _midi_note_off = false;
                    _midi_note = msg.msg.get_note();
                    _midi_note_on_time = midi_note_off_time;
                }
            }
        }
        
        if (_midi_note_off == false && msg.msg.is_note_off())
        {
            midi_note_off_time = timestamp + (float)msg.sample_position / (float)_sample_rate;
            if (_midi_note_on_time > midi_note_off_time)
            {
                _midi_note_on = false;
                _midi_note_on_time = 0;
            }
            else if (_midi_note_on && msg.msg.get_note() == _midi_note)
            {
                _midi_note_off = true;
            }
        }
        
        if (_midi_note_on && _midi_note_off)
        {
            std::shared_ptr<manual_tune::tune_node> tune(new manual_tune::tune_node());
            tune->time_start = _midi_note_on_time;
            tune->time_end = midi_note_off_time;
            tune->pitch_start = _midi_note - 69;
            tune->pitch_end = _midi_note - 69;
            tune->attack = attack;
            tune->release = release;
            tune->amount = amount;
            _m_tune.disable_history();
            _m_tune.add_tune(tune);
            _m_tune.enable_history();
            
            _midi_note_on = false;
            _midi_note_off = false;
            _midi_note = 0;
            _midi_note_on_time = 0;
        }
    }
}


std::list<mx_tune::midi_msg_node> mx_tune::output_midi_from_note(std::int32_t n, float timestamp)
{
    std::list<midi_msg_node> msg_list;
    float time_begin = timestamp;
    float time_end = timestamp + (float)n / (float)_sample_rate;

    std::list<std::shared_ptr<manual_tune::tune_node> > tune = _m_tune.get_tune(time_begin, time_end);
    for (const auto& i: tune)
    {
        std::int32_t on_sample_position = (i->time_start - timestamp) * (float)_sample_rate;
        std::int32_t off_sample_position = (i->time_end - timestamp) * (float)_sample_rate;
        if (on_sample_position >= 0 && on_sample_position < n)
        {
            midi_msg_node node;
            node.msg.note_on(1, (std::int32_t)i->pitch_start + 69);
            node.sample_position = on_sample_position;
            msg_list.push_back(node);
        }
        
        if (off_sample_position >= 0 && off_sample_position < n)
        {
            midi_msg_node node;
            node.msg.note_off(1, (std::int32_t)i->pitch_start + 69);
            node.sample_position = off_sample_position;
            msg_list.push_back(node);
        }
    }
    return msg_list;
}
    
void mx_tune::_set_detector(std::uint32_t detector_type)
{
    if (detector_type == DETECTOR_TYPE_TALENT)
    {
        _detector.reset(new pitch_detector_talent(_sample_rate));
        _detector->set_vthresh(_conf_detect_thresh);
        _detector->set_aref(_aref);
        _detector->set_freq_range(_det_min_freq, _det_max_freq);
        _detector->set_gate(_det_gate);
        _apply_misc_param();
        _detector_type = detector_type;
    }
    else if (detector_type == DETECTOR_TYPE_YIN_FAST)
    {
        _detector.reset(new pitch_detector_aubio(_sample_rate, "yinfast"));
        _detector->set_vthresh(_conf_detect_thresh);
        _detector->set_aref(_aref);
        _detector->set_freq_range(_det_min_freq, _det_max_freq);
        _detector->set_gate(_det_gate);
        _apply_misc_param();
        _detector_type = detector_type;
    }
    else if (detector_type == DETECTOR_TYPE_YIN)
    {
        _detector.reset(new pitch_detector_aubio(_sample_rate, "yin"));
        _detector->set_vthresh(_conf_detect_thresh);
        _detector->set_aref(_aref);
        _detector->set_freq_range(_det_min_freq, _det_max_freq);
        _detector->set_gate(_det_gate);
        _apply_misc_param();
        _detector_type = detector_type;
    }
}


void mx_tune::_set_shifter(std::uint32_t shifter_type)
{
    if (shifter_type == SHIFTER_TYPE_TALENT)
    {
        _shifter.reset(new pitch_shifter_talent(_sample_rate));
        _shifter->set_aref(_aref);
        _apply_misc_param();
        _shifter_type = shifter_type;
    }
    else if (shifter_type == SHIFTER_TYPE_SOUND_TOUCH)
    {
        _shifter.reset(new pitch_shifter_st(_sample_rate));
        _shifter->set_aref(_aref);
        _apply_misc_param();
        _shifter_type = shifter_type;
    }
    else if (shifter_type == SHIFTER_TYPE_RUBBERBAND)
    {
        _shifter.reset(new pitch_shifter_rb(_sample_rate));
        _shifter->set_aref(_aref);
        _apply_misc_param();
        _shifter_type = shifter_type;
    }
    else if (shifter_type == SHIFTER_TYPE_SMB)
    {
        _shifter.reset(new pitch_shifter_smb(_sample_rate));
        _shifter->set_aref(_aref);
        _apply_misc_param();
        _shifter_type = shifter_type;
    }
}
    
std::vector<std::string> mx_tune::_string_split(std::string str, const std::string& key)
{
    std::vector<std::string> out;
    std::string::size_type begin = 0;
    std::string::size_type end = 0;
    while ((end = str.find(key, begin)) != str.npos)
    {
        out.push_back(str.substr(begin, end));
        begin = end + key.size();
    }
    if (begin < str.size())
    {
        out.push_back(str.substr(begin, end));
    }
    
    return out;
}

void mx_tune::_apply_misc_param()
{
    text_readline readline;
    readline.load(_misc.c_str(), _misc.length());
    
    while (1)
    {
        std::string line = readline.read_line();
        if (line.length() == 0)
        {
            break;
        }
        
        std::vector<std::string> v_str = _string_split(line, "=");
        if (v_str.size() < 2)
        {
            continue;
        }
        
        if (_detector->set_misc_param(v_str[0].c_str(), v_str[1].c_str()))
        {
            continue;
        }
        if (_shifter->set_misc_param(v_str[0].c_str(), v_str[1].c_str()))
        {
            continue;
        }
        
        if (v_str[0] == "delay")
        {
            _delay.set_delay(std::atoi(v_str[1].c_str()));
            continue;
        }
    }
}