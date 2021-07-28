#include <math.h>
#include "auto_tune.h"

#define PI (float)3.14159265358979323846

auto_tune::auto_tune()
    : _num_notes(0)
    , _pull(0.)
    , _fixed(0.)
    , _shift(0.)
    , _smooth(1.0)
    , _amount(1.0)
    , _scwarp(0)
{
    for (int i = 0; i < 12; i++)
    {
        _notes[i] = 0;
        _pitch2note[i] = 0;
        _note2pitch[i] = 0;
    }
    
    _update();
}

auto_tune::~auto_tune()
{
}


void auto_tune::set_note(int notes[12])
{
    for (int i = 0; i < 12; i++)
    {
        _notes[i] = notes[i];
    }
    _update();
}

void auto_tune::_update()
{
    // Some logic for the semitone->scale and scale->semitone conversion
    // If no notes are selected as being in the scale, instead snap to all notes
    int ti2 = 0;
    for(int i = 0; i < 12; i++)
    {
        if(_notes[i] >= 0)
        {
            _pitch2note[i] = ti2;
            _note2pitch[ti2] = i;
            ti2 = ti2 + 1;
        }
        else
        {
            _pitch2note[i] = -1;
        }
    }
    
    _num_notes = ti2;
    while(ti2 < 12)
    {
        _note2pitch[ti2] = -1;
        ti2 = ti2 + 1;
    }
    if(_num_notes == 0)
    {
        for(int i = 0; i < 12; i++)
        {
            _notes[i] = 1;
            _pitch2note[i] = i;
            _note2pitch[i] = i;
        }
        _num_notes = 12;
    }
}



float auto_tune::tune(float inpitch)
{
    int lowersnap;
    int uppersnap;
    //float smooth = _smooth * 0.8;
    float smooth = _smooth;
    int scwarp = (_scwarp + _num_notes * 5) % _num_notes;
    
    //  ---- Modify pitch in all kinds of ways! ----

    float outpitch = inpitch;

    // Pull to fixed pitch
    outpitch = (1 - _pull) * outpitch + _pull * _fixed;

    // -- Convert from semitones to scale notes --
    int ti = (int)(outpitch / 12 + 32) - 32; // octave
    float tf = outpitch - ti * 12;             // semitone in octave
    int ti2 = (int)tf;
    int ti3 = ti2 + 1;
    
    // a little bit of pitch correction logic, since it's a convenient place for it
    if(_notes[ti2 % 12] < 0 || _notes[ti3 % 12] < 0)   // if between 2 notes that are more than a semitone apart
    {
        lowersnap = 1;
        uppersnap = 1;
    }
    else
    {
        lowersnap = 0;
        uppersnap = 0;
        if(_notes[ti2 % 12] == 1)   // if specified by user
        {
            lowersnap = 1;
        }
        if(_notes[ti3 % 12] == 1)   // if specified by user
        {
            uppersnap = 1;
        }
    }
    
    // (back to the semitone->scale conversion)
    // finding next lower pitch in scale
    while(_notes[(ti2 + 12) % 12] < 0)
    {
        ti2 = ti2 - 1;
    }
    
    // finding next higher pitch in scale
    while(_notes[ti3 % 12] < 0)
    {
        ti3 = ti3 + 1;
    }
    tf = (tf - ti2) / (ti3 - ti2) + _pitch2note[(ti2 + 12) % 12];
    if(ti2 < 0)
    {
        tf = tf - _num_notes;
    }
    
    outpitch = tf + _num_notes * ti;
    // -- Done converting to scale notes --

    // The actual pitch correction
    ti = (int)(outpitch + 128) - 128;
    tf = outpitch - ti - 0.5;
    ti2 = ti3 - ti2;
    float tf2;
    
    if(ti2 > 2)   // if more than 2 semitones apart, put a 2-semitone-like transition halfway between
    {
        tf2 = (float)ti2 / 2;
    }
    else
    {
        tf2 = (float)1;
    }
    
    if(smooth < 0.001)
    {
        tf2 = tf * tf2 / 0.001;
    }
    else
    {
        tf2 = tf * tf2 / smooth;
    }
    
    if(tf2 < -0.5)
        tf2 = -0.5;
    if(tf2 > 0.5)
        tf2 = 0.5;
    tf2 = 0.5 * sin(PI * tf2) + 0.5; // jumping between notes using horizontally-scaled sine segment
    tf2 = tf2 + ti;
    if((tf < 0.5 && lowersnap) || (tf >= 0.5 && uppersnap))
    {
        outpitch = _amount * tf2 + ((float)1 - _amount) * outpitch;
    }

    // Add in pitch shift
    outpitch = outpitch + _shift;

    // Convert back from scale notes to semitones
    outpitch = outpitch + scwarp; // output scale rotate implemented here
    ti = (int)(outpitch / _num_notes + 32) - 32;
    tf = outpitch - ti * _num_notes;
    ti2 = (int)tf;
    ti3 = ti2 + 1;
    outpitch = _note2pitch[ti3 % _num_notes] - _note2pitch[ti2];
    if(ti3 >= _num_notes)
    {
        outpitch = outpitch + 12;
    }
    outpitch = outpitch * (tf - ti2) + _note2pitch[ti2];
    outpitch = outpitch + 12 * ti;
    outpitch = outpitch - (_note2pitch[scwarp] - _note2pitch[0]); // more scale rotation here

    if(outpitch < -36)
        outpitch = -48;
    if(outpitch > 24)
        outpitch = 24;

    return outpitch;

    //  ---- END Modify pitch in all kinds of ways! ----
}
