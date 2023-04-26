/*
 * Copyright (C) 2023 Liu An Lin <liuanlin-mx@qq.com>
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