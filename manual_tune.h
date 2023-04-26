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


#ifndef __MANUAL_TUNE_H__
#define __MANUAL_TUNE_H__
#include <cstdint>
#include <memory>
#include <map>
#include <list>
#include <algorithm>
#include <math.h>
#include <float.h>
#include "auto_tune.h"
#include "undo_redo.h"

class manual_tune
{
public:
    enum
    {
        SELECT_NONE,
        SELECT_LEFT,
        SELECT_MID,
        SELECT_RIGHT,
    };
    
    struct pitch_node
    {
        float pitch = 0.;
        float conf = 0.;
        
        bool is_same(const pitch_node& other) const
        {
            return abs(other.pitch - pitch) < 0.0001 && abs(other.conf - conf) < 0.0001;
        }
    };
    
    struct tune_node
    {
        void reset()
        {
            time_start = 0.;
            time_end = 0.;
            pitch_start = 0.;
            pitch_end = 0.;
            attack = 0.;
            release = 0.;
            amount = 1.;
        }
        
        bool operator== (const tune_node& other)
        {
            return (is_manual == other.is_manual)
                    && (std::fabs(time_start - other.time_start) < FLT_EPSILON)
                    && (std::fabs(time_end - other.time_end) < FLT_EPSILON)
                    && (std::fabs(pitch_start - other.pitch_start) < FLT_EPSILON)
                    && (std::fabs(pitch_end - other.pitch_end) < FLT_EPSILON)
                    && (std::fabs(attack - other.attack) < FLT_EPSILON)
                    && (std::fabs(release - other.release) < FLT_EPSILON)
                    && (std::fabs(amount - other.amount) < FLT_EPSILON);
        }
        
        bool is_manual = false;
        float time_start = 0.;
        float time_end = 0.;
        float pitch_start = 0.;
        float pitch_end = 0.;
        float attack = 0.;
        float release = 0.;
        float amount = 1.;
    };
    
    
public:
    explicit manual_tune(std::uint32_t max_len = 10 * 60 * 1000);
    ~manual_tune();
    
private:
    manual_tune(const manual_tune&);
    manual_tune& operator=(const manual_tune&);
    
public:
    float get_time_len() { return _idx2time(_len); }
    
    void set_inpitch(float time_begin, float time_end, const pitch_node& node);
    pitch_node get_inpitch(float time);
    std::list<std::pair<pitch_node, float> > get_inpitch(float time_begin, float time_end);
    
    void set_outpitch(float time_begin, float time_end, const pitch_node& node);
    pitch_node get_outpitch(float time);
    std::list<std::pair<pitch_node, float> > get_outpitch(float time_begin, float time_end);
    
    void clear_inpitch();
    void clear_outpitch();
    
    void add_tune(std::shared_ptr<tune_node>& tune);
    std::list<std::shared_ptr<tune_node> > get_tune(float time_begin, float time_end);
    std::shared_ptr<tune_node> get_tune(float time);
    
    std::shared_ptr<tune_node> select_tune(float time, float pitch, std::uint32_t& pos);
    void unselect_tune();
    void del_selected();
    
    void clear_note();
    void clear_auto_note();
    
    void snap_key(const std::int32_t notes[12], float time_min_len, float time_max_interval, float attack, float release, float amount);
    void snap_to_inpitch();
    
    void enable_history();
    void disable_history();
    void undo();
    void redo();
    
    bool check_key(float notes_weights[12], float time_min_len, float time_max_interval);
    
    void set_vthresh(float thresh) { _conf_thresh = thresh; }
private:
    
    std::uint32_t _time2idx(float time);
    float _idx2time(std::uint32_t idx);
    void _write_tune(const std::shared_ptr<tune_node>& tune);
    void _erase_tune(const std::shared_ptr<tune_node>& tune);
    void _erase_tune_if(const std::shared_ptr<tune_node>& tune, bool b);
    void _remove_overlap(const std::shared_ptr<tune_node>& node);
    void _add_auto_tune(const std::shared_ptr<tune_node>& tune);
    
    void _linear_fit_from_inpitch(float time_begin, float time_end, float& a, float& b);
    float _tune2pitch(const std::shared_ptr<tune_node>& tune, float time, float inpitch, float a, float b);
    
    float _snap_pitch(const std::int32_t notes[12], float pitch);
    
    std::list<tune_node> _make_history();
    void _restor_history(const std::list<tune_node>& history);
    void _save_history();
    
private:
    pitch_node *_inpitch;
    pitch_node *_outpitch;
    std::shared_ptr<tune_node> *_tune_list;
    std::shared_ptr<tune_node> _select;
    tune_node _select_bak;
    std::uint32_t _max_len;
    std::uint32_t _len;
    float _min_time;
    float _conf_thresh;
    bool _enable_history;
    
    undo_redo<std::list<tune_node> > _history;
    
};

#endif