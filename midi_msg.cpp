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


#include "midi_msg.h"

midi_msg::midi_msg()
    : _channel(1)
    , _is_note_on(false)
    , _note(0)
    , _velocity(0)
{
}

midi_msg::~midi_msg()
{
}

void midi_msg::set_channel(std::int32_t channel)
{
    _channel = channel;
}

std::int32_t midi_msg::get_channel() const
{
    return _channel;
}

bool midi_msg::is_note_on() const
{
    return _is_note_on;
}

bool midi_msg::is_note_off() const
{
    return !_is_note_on;
}

std::int32_t midi_msg::get_note() const
{
    return _note;
}

std::int32_t midi_msg::get_velocity() const
{
    return _velocity;
}
    
void midi_msg::note_on(std::int32_t channel, std::int32_t note, std::int32_t velocity)
{
    _is_note_on = true;
    _channel = channel;
    _note = note;
    _velocity = velocity;
}

void midi_msg::note_off(std::int32_t channel, std::int32_t note, std::int32_t velocity)
{
    _is_note_on = false;
    _channel = channel;
    _note = note;
    _velocity = velocity;
}