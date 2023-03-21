#include <stdlib.h>
#include <delay.h>

delay::delay(std::uint32_t max_delay)
    : _max_delay(max_delay)
    , _delay(0)
    , _idx(0)
    , _data(NULL)
{
    _data = (float *)calloc(_max_delay, sizeof(float));
}

delay::~delay()
{
    free(_data);
}

void delay::set_delay(std::uint32_t delay)
{
    if (delay > _max_delay)
    {
        _delay = _max_delay;
    }
    else
    {
        _delay = delay;
    }
    
    for (std::uint32_t i = 0; i < _delay; i++)
    {
        _data[i] = 0.;
    }
}

float delay::process(float v)
{
    if (_delay == 0)
    {
        return v;
    }
    
    std::uint32_t idx = _idx % _delay;
    _idx++;
    float tmp = _data[idx];
    _data[idx] = v;
    return tmp;
}
