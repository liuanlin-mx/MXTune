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


#ifndef __DELAY_H__
#define __DELAY_H__
#include <cstdint>

class delay
{
public:
    explicit delay(std::uint32_t max_delay = 44100);
    ~delay();
    
public:
    void set_delay(std::uint32_t delay);
    std::uint32_t get_delay() { return _delay; }
    float process(float v);
    
private:
    std::uint32_t _max_delay;
    std::uint32_t _delay;
    std::uint32_t _idx;
    float *_data;
};

#endif