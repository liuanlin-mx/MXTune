#ifndef __MANUAL_TUNE_H__
#define __MANUAL_TUNE_H__
#include <memory>
#include <map>
#include <list>
#include <math.h>
#include "auto_tune.h"

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
        
        bool is_same(const pitch_node& other)
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
        
        float time_start = 0.;
        float time_end = 0.;
        float pitch_start = 0.;
        float pitch_end = 0.;
        float attack = 0.;
        float release = 0.;
        float amount = 1.;
    };
    
    
public:
    manual_tune(unsigned int len = 10 * 60 * 1000);
    ~manual_tune();
    
    
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
    
    void set_tune(std::shared_ptr<tune_node>& tune);
    std::list<std::shared_ptr<tune_node> > get_tune(float time_begin, float time_end);
    std::shared_ptr<tune_node> get_tune(float time);
    std::shared_ptr<tune_node> select_tune(float time, float pitch, std::uint32_t& pos);
    void unselect_tune();
    void del_selected();
    void clear_note();
    
    void snap_key(auto_tune *tune);
    
    void set_vthresh(float thresh) { _conf_thresh = thresh; }
private:
    
    std::uint32_t _time2idx(float time);
    float _idx2time(std::uint32_t idx);
    void _write_tune(const std::shared_ptr<tune_node>& tune);
    void _erase_tune(const std::shared_ptr<tune_node>& tune);
    void _erase_tune_if(const std::shared_ptr<tune_node>& tune, bool b);
    void _remove_overlap(const std::shared_ptr<tune_node>& node);
    
    void _linear_fit_from_inpitch(float time_begin, float time_end, float& a, float& b);
    float _tune2pitch(const std::shared_ptr<tune_node>& tune, float time, float inpitch, float a, float b);
    
private:
    pitch_node *_inpitch;
    pitch_node *_outpitch;
    std::shared_ptr<tune_node> *_tune_list;
    std::shared_ptr<tune_node> _select;
    std::uint32_t _max_len;
    std::uint32_t _len;
    float _min_time;
    float _conf_thresh;
    
};

#endif