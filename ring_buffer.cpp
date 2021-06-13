#include <malloc.h>
#include "ring_buffer.h"

ring_buffer::ring_buffer(unsigned int sample_rate)
{
    _buf_size = get_size_from_rate(sample_rate);
    
    _corr_size = _buf_size / 2 + 1;

    _buf_i = (float *)calloc(_buf_size, sizeof(float));
    //_buf_f = (float *)calloc(_buf_size, sizeof(float));
    _idx = 0;
}

ring_buffer::~ring_buffer()
{
    free(_buf_i);
    //free(_buf_f);
}
