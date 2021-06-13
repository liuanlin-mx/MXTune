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