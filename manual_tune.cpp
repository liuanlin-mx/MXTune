#define _USE_MATH_DEFINES
#include <math.h>
#include "manual_tune.h"
#include "net_log.h"

manual_tune::manual_tune(unsigned int len)
{
    _max_len = len;
    _len = 0;
    _inpitch = new pitch_node[len];
    _outpitch = new pitch_node[len];
    _tune_list = new std::shared_ptr<tune_node>[len];
    
    _min_time = 0.01;
    _conf_thresh = 0.7;
}

manual_tune::~manual_tune()
{
    delete [] _inpitch;
    delete [] _outpitch;
    delete [] _tune_list;
}

void manual_tune::set_inpitch(float time_begin, float time_end, const pitch_node& node)
{
    int begin = _time2idx(time_begin);
    int end = _time2idx(time_end);
    
    if (end > _len)
    {
        _len = end;
    }
    
    for (int i = begin; i < end; i++)
    {
        _inpitch[i] = node;
    }
}

manual_tune::pitch_node manual_tune::get_inpitch(float time)
{
    unsigned int idx = _time2idx(time);
    return _inpitch[idx];
}


std::list<std::pair<manual_tune::pitch_node, float> > manual_tune::get_inpitch(float time_begin, float time_end)
{
    int begin = _time2idx(time_begin);
    int end = _time2idx(time_end);
    
    std::list<std::pair<manual_tune::pitch_node, float> > list;
    pitch_node last;
    for (int i = begin; i < end; i++)
    {
        pitch_node& tmp = _inpitch[i];
            
        if (!tmp.is_same(last))
        {
            last = tmp;
            list.push_back(std::pair<manual_tune::pitch_node, float>(tmp, _idx2time(i)));
        }
    }
    return std::move(list);
}

void manual_tune::set_outpitch(float time_begin, float time_end, const pitch_node& node)
{
    int begin = _time2idx(time_begin);
    int end = _time2idx(time_end);
    
    if (end > _len)
    {
        _len = end;
    }
    
    for (int i = begin; i < end; i++)
    {
        _outpitch[i] = node;
    }
}

manual_tune::pitch_node manual_tune::get_outpitch(float time)
{
    unsigned int idx = _time2idx(time);
    
    pitch_node& outpitch = _outpitch[idx];
    std::shared_ptr<tune_node>& tune = _tune_list[idx];
    if (tune)
    {
        float a = 0;
        float b = 0;
        pitch_node tmp;
        const pitch_node& inpitch = _inpitch[idx];
        tmp.conf = inpitch.conf;
        if (1 || inpitch.conf >= _conf_thresh)
        {
            _linear_fit_from_inpitch(tune->time_start, tune->time_end, a, b);
            tmp.pitch = _tune2pitch(tune, time, inpitch.pitch, a, b);
        }
        else
        {
            tmp.pitch = inpitch.pitch;
        }
        return tmp;
    }
    
    return outpitch;
}

std::list<std::pair<manual_tune::pitch_node, float> > manual_tune::get_outpitch(float time_begin, float time_end)
{
    int begin = _time2idx(time_begin);
    int end = _time2idx(time_end);
    
    
    std::list<std::pair<manual_tune::pitch_node, float> > list;
    pitch_node last;
    
    float a = 0;
    float b = 0;
    std::shared_ptr<tune_node> last_tune;
    
    for (int i = begin; i < end; i++)
    {
        pitch_node& inpitch = _inpitch[i];
        pitch_node& outpitch = _outpitch[i];
        std::shared_ptr<tune_node> tune = _tune_list[i];
        
        if (_select)
        {
            if (i >= _time2idx(_select->time_start) && i < _time2idx(_select->time_end))
            {
                tune = _select;
            }
        }
        
        if (tune)
        {
            pitch_node tmp;
            float time = _idx2time(i);
            tmp.conf = inpitch.conf;
            
            if (1 || inpitch.conf >= _conf_thresh)
            {
                if (tune != last_tune)
                {
                    _linear_fit_from_inpitch(tune->time_start, tune->time_end, a, b);
                }
                
                tmp.pitch = _tune2pitch(tune, time, inpitch.pitch, a, b);
                last_tune = tune;
            }
            else
            {
                tmp.pitch = tmp.pitch;
            }
            
            if (!tmp.is_same(last))
            {
                last = tmp;
                list.push_back(std::pair<manual_tune::pitch_node, float>(tmp, time));
            }
        }
        else
        {
            pitch_node& tmp = _outpitch[i];
            if (!tmp.is_same(last))
            {
                last = tmp;
                list.push_back(std::pair<manual_tune::pitch_node, float>(tmp, _idx2time(i)));
            }
        }
    }
    return std::move(list);
}


void manual_tune::clear_inpitch()
{
    pitch_node tmp;
    for (std::int32_t i = 0; i < _max_len; i++)
    {
        _inpitch[i] = tmp;
    }
}

void manual_tune::clear_outpitch()
{
    pitch_node tmp;
    for (std::int32_t i = 0; i < _max_len; i++)
    {
        _outpitch[i] = tmp;
    }
}
    
void manual_tune::set_tune(std::shared_ptr<tune_node>& tune)
{
    if (abs(tune->time_end - tune->time_start) < _min_time)
    {
        return;
    }
    _write_tune(tune);
    _remove_overlap(tune);
}

std::list<std::shared_ptr<manual_tune::tune_node> > manual_tune::get_tune(float time_begin, float time_end)
{
    std::uint32_t idx_begin = _time2idx(time_begin);
    std::uint32_t idx_end = _time2idx(time_end);
    std::shared_ptr<tune_node> last;
    std::list<std::shared_ptr<tune_node> > list;
    while (idx_begin < idx_end)
    {
        std::shared_ptr<tune_node>& node = _tune_list[idx_begin];
        if (node != nullptr && node != last)
        {
            list.push_back(node);
            last = node;
        }
        idx_begin++;
    }
    return list;
}

std::shared_ptr<manual_tune::tune_node> manual_tune::get_tune(float time)
{
    std::uint32_t idx = _time2idx(time);
    return _tune_list[idx];
}

std::shared_ptr<manual_tune::tune_node> manual_tune::select_tune(float time, float pitch, std::uint32_t& pos)
{
    std::uint32_t idx = _time2idx(time);
    std::shared_ptr<tune_node>& node = _tune_list[idx];
    if (node && (abs(pitch - node->pitch_start) < 0.5 || abs(pitch - node->pitch_end) < 0.5))
    {
        float len = node->time_end - node->time_start;
        float left = node->time_start + len * 0.2;
        float right = node->time_end - len * 0.2;
        
        if (time <= left)
        {
            pos = SELECT_LEFT;
        }
        else if (time >= right)
        {
            pos = SELECT_RIGHT;
        }
        else
        {
            pos = SELECT_MID;
        }
        
        _select = node;
        _erase_tune(node);
        return _select;
    }
    return std::shared_ptr<tune_node>();
}

void manual_tune::unselect_tune()
{
    if (_select)
    {
        if (abs(_select->time_end - _select->time_start) >= _min_time)
        {
            _write_tune(_select);
            _remove_overlap(_select);
        }
        _select.reset();
    }
}

void manual_tune::del_selected()
{
    _select.reset();
}

void manual_tune::clear_note()
{
    for (std::int32_t i = 0; i < _max_len; i++)
    {
        _tune_list[i].reset();
    }
}

void manual_tune::snap_key(auto_tune *tune)
{
    for (std::int32_t i = 0; i < _len; i++)
    {
        _outpitch[i].pitch = tune->tune(_inpitch[i].pitch);
        _outpitch[i].conf = _inpitch[i].conf;
    }
}

std::uint32_t manual_tune::_time2idx(float time)
{
    if (time < 0.)
    {
        return 0;
    }
    std::uint32_t idx = round(time * 1000);
    if (idx >= _max_len)
    {
        idx = _max_len;
    }
    return idx;
}

float manual_tune::_idx2time(std::uint32_t idx)
{
    return idx / 1000.;
}

void manual_tune::_write_tune(const std::shared_ptr<tune_node>& tune)
{
    if (tune->time_start > tune->time_end)
    {
        std::swap(tune->time_start, tune->time_end);
        std::swap(tune->pitch_start, tune->pitch_end);
    }
    
    if (tune->time_end - tune->time_start < 0.0001)
    {
        return;
    }
    
    std::uint32_t idx_begin = _time2idx(tune->time_start);
    std::uint32_t idx_end = _time2idx(tune->time_end);
    
    while (idx_begin < idx_end)
    {
        _tune_list[idx_begin] = tune;
        idx_begin++;
    }
}

void manual_tune::_erase_tune(const std::shared_ptr<tune_node>& tune)
{
    std::uint32_t idx_begin = _time2idx(tune->time_start);
    std::uint32_t idx_end = _time2idx(tune->time_end);
    
    if (idx_begin > idx_end)
    {
        std::swap(idx_begin, idx_end);
    }
    
    while (idx_begin < idx_end)
    {
        _tune_list[idx_begin] = nullptr;
        idx_begin++;
    }
}

void manual_tune::_erase_tune_if(const std::shared_ptr<tune_node>& tune, bool b)
{
    if (b)
    {
        _erase_tune(tune);
    }
}

void manual_tune::_remove_overlap(const std::shared_ptr<tune_node>& tune)
{
    std::uint32_t idx_begin = _time2idx(tune->time_start);
    std::uint32_t idx_end = _time2idx(tune->time_end);
    std::shared_ptr<tune_node> left;
    std::shared_ptr<tune_node> right;
    if (idx_begin > 0)
    {
        
        left = _tune_list[idx_begin - 1];
    }
    
    if (idx_end < _max_len)
    {
        right = _tune_list[idx_end + 1];
    }
    
    bool flag = false;
    if (right && left && right == left)
    {
        flag = true;
        right = std::shared_ptr<tune_node>(new tune_node(*(right.get())));
    }
    
    if (left && left != tune && left->time_end > tune->time_start)
    {
        left->time_end = tune->time_start;
    }
    
    if (right && right != tune && right->time_start < tune->time_end)
    {
        right->time_start = tune->time_end;
    }
    
    if (flag)
    {
        _write_tune(right);
    }
    _erase_tune_if(left, left && left->time_end - left->time_start < _min_time);
    _erase_tune_if(right, right && right->time_end - right->time_start < _min_time);
}


void manual_tune::_linear_fit_from_inpitch(float time_begin, float time_end, float& a, float& b)
{
    float x_sum = 0;
    float y_sum = 0;
    float x2_sum = 0;
    float xy_sum = 0;
    
    std::int32_t begin = _time2idx(time_begin);
    std::int32_t end = _time2idx(time_end);
    std::int32_t n = end - begin;
    
    for (std::int32_t i = begin; i < end; i++)
    {
        if (_inpitch[i].conf < _conf_thresh)
        {
            continue;
        }
        
        float time = _idx2time(i);
        x_sum += time;
        y_sum += _inpitch[i].pitch;
        x2_sum += time * time;
        xy_sum += time * _inpitch[i].pitch;
    }
    
    a = (n * xy_sum - x_sum * y_sum) / (n * x2_sum - x_sum * x_sum);
    b = (y_sum - a * x_sum) / n;
}

float manual_tune::_tune2pitch(const std::shared_ptr<tune_node>& tune, float time, float inpitch, float a, float b)
{
    //y = y0 + (y1 - y0) / (x1 - x0) * (x - x0)
    
    float x = time;
    
    float x0 = tune->time_start;
    float y0 = tune->pitch_start;
    
    float x1 = tune->time_end;
    float y1 = tune->pitch_end;
    float pitch = y0 + (y1 - y0) / (x1 - x0) * (x - x0);
    
    float pitch_fit = a * time + b;
    
    float k = 1.0;
    if (time < tune->time_start + tune->attack)
    {
        float t = time - tune->time_start;
        k = sin(t * M_PI_2 / tune->attack);
    }
    else if (time >= tune->time_end - tune->release)
    {
        float t = time - (tune->time_end - tune->release);
        k = 1. - sin(t * M_PI_2 / tune->release);
    }
    
    pitch_fit = (pitch_fit - pitch) * k + pitch;
    return (inpitch - pitch_fit) * (1.0 - tune->amount * k) + pitch;
}