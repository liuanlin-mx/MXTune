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


#ifndef __MIDI_MSG_H__
#define __MIDI_MSG_H__
#include <cstdint>

class midi_msg
{
    /*
     * channel  1 to 16
     * note     0 to 127
     * velocity 0 to 127
    */
public:
    midi_msg();
    ~midi_msg();
    
public:
    void set_channel(std::int32_t channel);
    std::int32_t get_channel() const;
    
    bool is_note_on() const;
    bool is_note_off() const;
    std::int32_t get_note() const;
    std::int32_t get_velocity() const;
    void note_on(std::int32_t channel, std::int32_t note, std::int32_t velocity = 80);
    void note_off(std::int32_t channel, std::int32_t note, std::int32_t velocity = 80);
    
private:
    std::int32_t _channel;
    bool _is_note_on;
    std::int32_t _note;
    std::int32_t _velocity;
};

#endif 