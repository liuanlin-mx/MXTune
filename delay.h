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