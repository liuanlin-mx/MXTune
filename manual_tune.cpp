#define _USE_MATH_DEFINES
#include <math.h>
#include "manual_tune.h"

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
    std::uint32_t idx = _time2idx(time);
    
    {
        const pitch_node& inpitch = _inpitch[idx];
        while (idx > 0 && inpitch.is_same(_inpitch[idx - 1]))
        {
            --idx;
        }
    }
    
    time = _idx2time(idx);
    
    const pitch_node& outpitch = _outpitch[idx];
    std::shared_ptr<tune_node>& tune = _tune_list[idx];
    if (tune)
    {
        float a = 0;
        float b = 0;
        
        pitch_node tmp;
        const pitch_node& inpitch = _inpitch[idx];
        tmp.conf = inpitch.conf;
        _linear_fit_from_inpitch(tune->time_start, tune->time_end, a, b);
        tmp.pitch = _tune2pitch(tune, time, inpitch.pitch, a, b);
        return tmp;
    }
    
    return outpitch;
}

std::list<std::pair<manual_tune::pitch_node, float> > manual_tune::get_outpitch(float time_begin, float time_end)
{
    int begin = _time2idx(time_begin);
    int end = _time2idx(time_end);
    
    
    std::list<std::pair<manual_tune::pitch_node, float> > list;
    //pitch_node last;
    pitch_node last_inpitch;
    
    float a = 0;
    float b = 0;
    std::shared_ptr<tune_node> last_tune;
    
    for (int i = begin; i < end; i++)
    {
        pitch_node& inpitch = _inpitch[i];
        pitch_node& outpitch = _outpitch[i];
        std::shared_ptr<tune_node> tune = _tune_list[i];
        
        if (inpitch.is_same(last_inpitch))
        {
            continue;
        }
        last_inpitch = inpitch;
        
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
            
            if (tune != last_tune)
            {
                _linear_fit_from_inpitch(tune->time_start, tune->time_end, a, b);
            }
            last_tune = tune;
            
            tmp.pitch = _tune2pitch(tune, time, inpitch.pitch, a, b);
            list.push_back(std::pair<manual_tune::pitch_node, float>(tmp, time));
        }
        else
        {
            list.push_back(std::pair<manual_tune::pitch_node, float>(_outpitch[i], _idx2time(i)));
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
    if (node == nullptr)
    {
        pos = SELECT_NONE;
        return std::shared_ptr<tune_node>();
    }
    std::uint32_t begin = _time2idx(node->time_start);
    std::uint32_t end = _time2idx(node->time_end);
    
    float x0 = node->time_start;
    float y0 = node->pitch_start;
    float x1 = node->time_end;
    float y1 = node->pitch_end;
    
    float y = y0 + (y1 - y0) / (x1 - x0) * (time - x0);
    
    if (abs(y - pitch) < 0.2)
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



void manual_tune::snap_key(const std::int32_t notes[12], float time_min_len, float time_max_interval, float attack, float release, float amount)
{
    pitch_node last;
    bool flag = true;
    std::uint32_t idx_begin = 0;
    std::uint32_t idx_end = 0;
    float pitch = 0.;
    
    
    for (std::int32_t i = 0; i < _max_len; i++)
    {
        _tune_list[i].reset();
    }
    
    for (std::int32_t i = 0; i < _len; i++)
    {
        const pitch_node& inpitch = _inpitch[i];
        
        if (last.is_same(inpitch))
        {
            continue;
        }
        last = inpitch;
        
        bool end = false;
        if (inpitch.conf < _conf_thresh)
        {
            if (flag)
            {
                continue;
            }
            end = true;
        }
        
        float snap_pitch = _snap_pitch(notes, inpitch.pitch);
        
        if (flag)
        {
            idx_begin = i;
            pitch = snap_pitch;
            flag = false;
        }
        
        if ((std::int32_t)snap_pitch == (std::int32_t)pitch)
        {
            idx_end = i + 1;
        }
        else
        {
            float time = _idx2time(i);
            float time_end = _idx2time(idx_end);
            if (time - time_end > time_max_interval || fabs(snap_pitch - pitch) > 1.0)
            {
                end = true;
            }
        }
        
        if (end)
        {
            flag = true;
            float time_start = _idx2time(idx_begin);
            float time_end = _idx2time(idx_end);
            if (time_end - time_start > time_min_len)
            {
                std::shared_ptr<tune_node> tune(new tune_node);
                tune->time_start = time_start;
                tune->time_end = time_end;
                tune->pitch_start = pitch;
                tune->pitch_end = pitch;
                tune->attack = attack;
                tune->release = release;
                tune->amount = amount;
                
                _write_tune(tune);
                _remove_overlap(tune);
            }
            i = idx_end;
        }
    }
}

void manual_tune::snap_to_inpitch()
{
    for (std::int32_t i = 0; i < (std::int32_t)_len; i++)
    {
        _outpitch[i].pitch = _inpitch[i].pitch;
        _outpitch[i].conf = _inpitch[i].conf;
    }
}

bool manual_tune::check_key(std::int32_t notes[12], float time_min_len, float time_max_interval)
{
    pitch_node last;
    bool flag = true;
    std::uint32_t idx_begin = 0;
    std::uint32_t idx_end = 0;
    float pitch = 0.;
    
    std::int32_t notes_chromatic[12] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    std::uint32_t notes_count[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::int32_t notes_[12] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    
    
    
    for (std::int32_t i = 0; i < _len; i++)
    {
        const pitch_node& inpitch = _inpitch[i];
        
        if (last.is_same(inpitch))
        {
            continue;
        }
        last = inpitch;
        
        bool end = false;
        if (inpitch.conf < _conf_thresh)
        {
            if (flag)
            {
                continue;
            }
            end = true;
        }
        
        float snap_pitch = _snap_pitch(notes_chromatic, inpitch.pitch);
        
        if (flag)
        {
            idx_begin = i;
            pitch = snap_pitch;
            flag = false;
        }
        
        if ((std::int32_t)snap_pitch != (std::int32_t)pitch)
        {
            idx_end = i;
            flag = true;
            float time_start = _idx2time(idx_begin);
            float time_end = _idx2time(idx_end);
            if (time_end - time_start > time_min_len)
            {
                notes_count[(std::uint32_t)(pitch + 12 * 8) % 12]++;
            }
        }
    }
    
    for (std::int32_t i = 0; i < 7; i++)
    {
        std::uint32_t max = 0;
        std::int32_t max_idx = -1; 
        for (std::int32_t j = 0; j < 12; j++)
        {
            if (notes_count[j] > max)
            {
                max = notes_count[j];
                max_idx = j;
            }
        }
        if (max_idx < 0)
        {
            return false;
        }
        notes_count[max_idx] = 0;
        notes_[max_idx] = 1;
    }
    
    for (std::int32_t i = 0; i < 12; i++)
    {
        notes[i] = notes_[i];
    }
    
    return true;
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
        idx = _max_len - 1;
    }
    return idx;
}

float manual_tune::_idx2time(std::uint32_t idx)
{
    if (idx >= _max_len)
    {
        idx = _max_len - 1;
    }
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



float manual_tune::_snap_pitch(const std::int32_t notes[12], float pitch)
{
    std::int32_t i = round(pitch);
    std::int32_t left = i;
    std::int32_t right = i;

    for (std::int32_t j = 0; j < 12; j++)
    {
        if (notes[(left + 12 * 8) % 12] < 0)
        {
            left--;
        }
        else
        {
            break;
        }
    }

    for (std::int32_t j = 0; j < 11; j++)
    {
        if (notes[(right + 12 * 8) % 12] < 0)
        {
            right++;
        }
        else
        {
            break;
        }
    }

    if (abs(i - left) > abs(right - i))
    {
        return left;
    }
    else
    {
        return right;
    }
}