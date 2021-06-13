#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

struct ring_buffer
{
public:
    ring_buffer(unsigned int sample_rate);
    ~ring_buffer();
    
public:
    static unsigned int get_size_from_rate(unsigned int sample_rate)
    {
        return (sample_rate >= 88200)? 4096: 2048;
    }
    
public:
    inline void put(float v)
    {
        _buf_i[_idx] = v;
        if (++_idx >= _buf_size)
        {
            _idx = 0;
        }
    }
    
    inline float& operator[] (unsigned int idx)
    {
        return _buf_i[idx];
    }
    
    inline unsigned int get_idx()
    {
        return _idx;
    }
    
    inline float current() const
    {
        return _buf_i[_idx];
    }
    
    inline unsigned int get_buf_size()
    {
        return _buf_size;
    }
    
    inline unsigned int get_corr_size()
    {
        return _corr_size;
    }
    
private:
    unsigned int _buf_size;    // size of circular buffer
    unsigned int _corr_size;   // _buf_size / 2 + 1
    unsigned int _idx;         //write pointer;
    float* _buf_i;
    //float* _buf_f;              // circular formant correction buffer
};

#endif