#ifndef __MIDI_MSG_H__
#define __MIDI_MSG_H__
#include <cstdint>

class midi_msg
{
    /*
     * channel  1 to 16
     * note     0 to 127
     * velocity 0 to 127
    */
public:
    midi_msg();
    ~midi_msg();
    
public:
    void set_channel(std::int32_t channel);
    std::int32_t get_channel() const;
    
    bool is_note_on() const;
    bool is_note_off() const;
    std::int32_t get_note() const;
    std::int32_t get_velocity() const;
    void note_on(std::int32_t channel, std::int32_t note, std::int32_t velocity = 80);
    void note_off(std::int32_t channel, std::int32_t note, std::int32_t velocity = 80);
    
private:
    std::int32_t _channel;
    bool _is_note_on;
    std::int32_t _note;
    std::int32_t _velocity;
};

#endif 