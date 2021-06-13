#include "autotalent.h"
#include "mat_helper.h"

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
    
            static float test_in[32 * 1024];
            static float test_out[32 * 1024];
            static float test_conf[32 * 1024];
            static unsigned int test_idx = 0;
            test_in[test_idx] = inpitch;
            test_out[test_idx] = outpitch;
            test_conf[test_idx] = conf;
            test_idx++;
            if (test_idx >= sizeof(test_in) / sizeof(test_in[0]))
            {
                test_idx = 0;
            }
            
            if (test_idx % 1000 == 0)
            {
                mat_helper_write_mat2_4("inpitch", 1, test_idx, "float", test_in);
                mat_helper_write_mat2_4("outpitch", 1, test_idx, "float", test_out);
                mat_helper_write_mat2_4("conf", 1, test_idx, "float", test_conf);
            }
        }
        out[i] = _shifter.shifter(_buffer);
    }
}
