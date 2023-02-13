#ifndef __UNDO_REDO_H__
#define __UNDO_REDO_H__
#include <cstdint>
#include <deque>

template <typename T>

class undo_redo
{
public:
    undo_redo(std::uint32_t max = 30)
        : _max(max)
    {
        
    }
    
    ~undo_redo()
    {
        
    }
    
public:
    void put(const T& history)
    {
        _redo.clear();
        if (_undo.size() >= _max)
        {
            _undo.pop_front();
        }
        _undo.push_back(history);
    }
    
    bool undo(T& history, const T& current_status)
    {
        if (_undo.empty())
        {
            return false;
        }
        
        _redo.push_back(current_status);
        history = _undo.back();
        _undo.pop_back();
        return true;
    }
    
    bool redo(T& history, const T& current_status)
    {
        if (_redo.empty())
        {
            return false;
        }
        
        _undo.push_back(current_status);
        history = _redo.back();
        _redo.pop_back();
        return true;
    }
    
private:
    std::uint32_t _max;
    std::deque<T> _undo;
    std::deque<T> _redo;
    T _current_status;
};

#endif