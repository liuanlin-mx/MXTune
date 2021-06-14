#include "autotalent.h"

autotalent::autotalent(unsigned int sample_rate)
    : _detector(sample_rate)
    , _shifter(sample_rate)
    , _buffer(sample_rate)
{
    _noverlap = 4;
}

autotalent::~autotalent()
{
}

void autotalent::set_aref(float aref)
{
    _detector.set_aref(aref);
    _shifter.set_aref(aref);
}

void autotalent::set_mix(float mix)
{
    _shifter.set_mix(mix);
}

void autotalent::set_note(int notes[12])
{
    _tune.set_note(notes);
}

    void update_tune();
void autotalent::set_pull(float pull)
{
    _tune.set_pull(pull);
}

void autotalent::set_fixed(float fixed)
{
    _tune.set_fixed(fixed);
}

void autotalent::set_shift(float shift)
{
    _tune.set_shift(shift);
}

void autotalent::set_smooth(float smooth)
{
    _tune.set_smooth(smooth);
}

void autotalent::set_amount(float amount)
{
    _tune.set_amount(amount);
}

void autotalent::set_scwarp(int scwarp)
{
    _tune.set_scwarp(scwarp);
}

void autotalent::run(float* in, float *out, int n)
{
    
    for (int i = 0; i < n; i++)
    {
        _buffer.put(in[i]);
        if(_buffer.get_idx() % (_buffer.get_buf_size() / _noverlap) == 0)
        {
            float conf;
            float inpitch = _detector.get_period(_buffer, conf);
            float outpitch = _tune.tune(inpitch);
            _shifter.update_shifter_variables(inpitch, outpitch);
        }
        out[i] = _shifter.shifter(_buffer);
    }
}
