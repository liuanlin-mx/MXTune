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


#ifndef __AUTO_TUNE_H__
#define __AUTO_TUNE_H__
class auto_tune
{
public:
    auto_tune();
    ~auto_tune();
    
public:
    void set_note(int notes[12]);
    
    void set_pull(float pull) { _pull = pull; }
    void set_fixed(float fixed) { _fixed = fixed; }
    void set_shift(float shift) { _shift = shift; }
    void set_smooth(float smooth) { _smooth = smooth; }
    void set_amount(float amount) { _amount = amount; }
    void set_scwarp(int scwarp) { _scwarp = scwarp; }
    float tune(float inpitch);
    
private:
    void _update();
    
private:
    int _notes[12];
    int _pitch2note[12];
    int _note2pitch[12];
    int _num_notes;
    
    float _pull;
    float _fixed;
    float _shift;
    
    float _smooth;
    float _amount;
    int _scwarp;
};

#endif