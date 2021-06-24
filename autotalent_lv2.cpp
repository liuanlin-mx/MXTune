#include <lv2.h>
#include "autotalent.h"

// The port numbers

#define AT_TUNE 0
#define AT_FIXED 1
#define AT_PULL 2
#define AT_A 3
#define AT_Bb 4
#define AT_B 5
#define AT_C 6
#define AT_Db 7
#define AT_D 8
#define AT_Eb 9
#define AT_E 10
#define AT_F 11
#define AT_Gb 12
#define AT_G 13
#define AT_Ab 14
#define AT_AMOUNT 15
#define AT_SMOOTH 16
#define AT_SHIFT 17
#define AT_SCWARP 18
#define AT_LFOAMP 19
#define AT_LFORATE 20
#define AT_LFOSHAPE 21
#define AT_LFOSYMM 22
#define AT_LFOQUANT 23
#define AT_FCORR 24
#define AT_FWARP 25
#define AT_MIX 26
#define AT_PITCH 27
#define AT_CONF 28
#define AT_INPUT1 29
#define AT_OUTPUT1 30
#define AT_LATENCY 31


struct autotalent_lv2
{
    autotalent_lv2(unsigned int sample_rate)
        : _talent(sample_rate)
    {
        
    }
    
    autotalent _talent;
    
    
    float* _tune;
    float* _fixed;
    float* _pull;
    float* _A;
    float* _Bb;
    float* _B;
    float* _C;
    float* _Db;
    float* _D;
    float* _Eb;
    float* _E;
    float* _F;
    float* _Gb;
    float* _G;
    float* _Ab;
    float* _amount;
    float* _smooth;
    float* _shift;
    float* _scwarp;
    float* _lfoamp;
    float* _lforate;
    float* _lfoshape;
    float* _lfosymm;
    float* _lfoquant;
    float* _fcorr;
    float* _fwarp;
    float* _mix;
    float* _pitch;
    float* _conf;
    float* _input_buffer1;
    float* _output_buffer1;
    float* _latency;
};


static LV2_Handle instantiate(const LV2_Descriptor* descriptor, double rate,
                              const char* bundle_path, const LV2_Feature* const* features)
{
    return new autotalent_lv2(rate);
}

static void cleanup(LV2_Handle instance)
{
    autotalent_lv2 *talent = (autotalent_lv2 *)instance;
    delete talent;
}

//  Connect port
static void connect_port(LV2_Handle instance, uint32_t port, void* data_)
{
    float *data = (float *)data_;
    autotalent_lv2* talent = (autotalent_lv2*)instance;
    switch(port)
    {
    case AT_TUNE:
        talent->_tune = data;
        break;
    case AT_FIXED:
        talent->_fixed = data;
        break;
    case AT_PULL:
        talent->_pull = data;
        break;
    case AT_A:
        talent->_A = data;
        break;
    case AT_Bb:
        talent->_Bb = data;
        break;
    case AT_B:
        talent->_B = data;
        break;
    case AT_C:
        talent->_C = data;
        break;
    case AT_Db:
        talent->_Db = data;
        break;
    case AT_D:
        talent->_D = data;
        break;
    case AT_Eb:
        talent->_Eb = data;
        break;
    case AT_E:
        talent->_E = data;
        break;
    case AT_F:
        talent->_F = data;
        break;
    case AT_Gb:
        talent->_Gb = data;
        break;
    case AT_G:
        talent->_G = data;
        break;
    case AT_Ab:
        talent->_Ab = data;
        break;
    case AT_AMOUNT:
        talent->_amount = data;
        break;
    case AT_SMOOTH:
        talent->_smooth = data;
        break;
    case AT_SHIFT:
        talent->_shift = data;
        break;
    case AT_SCWARP:
        talent->_scwarp = data;
        break;
    case AT_LFOAMP:
        talent->_lfoamp = data;
        break;
    case AT_LFORATE:
        talent->_lforate = data;
        break;
    case AT_LFOSHAPE:
        talent->_lfoshape = data;
        break;
    case AT_LFOSYMM:
        talent->_lfosymm = data;
        break;
    case AT_LFOQUANT:
        talent->_lfoquant = data;
        break;
    case AT_FCORR:
        talent->_fcorr = data;
        break;
    case AT_FWARP:
        talent->_fwarp = data;
        break;
    case AT_MIX:
        talent->_mix = data;
        break;
    case AT_PITCH:
        talent->_pitch = data;
        break;
    case AT_CONF:
        talent->_conf = data;
        break;
    case AT_INPUT1:
        talent->_input_buffer1 = data;
        break;
    case AT_OUTPUT1:
        talent->_output_buffer1 = data;
        break;
    case AT_LATENCY:
        talent->_latency = data;
        if (talent->_latency)
        {
            *(talent->_latency) = talent->_talent.get_latency();
        }
        break;
    }
}

static void activate(LV2_Handle instance)
{
}

static void deactivate(LV2_Handle instance)
{
}

static const void* extension_data(const char* uri)
{
    return NULL;
}

static void run(LV2_Handle instance, uint32_t n_samples)
{
    float* input;
    float* output;
    
    float aref;
    float amount;
    float smooth;
    int notes[12];
    float fixed;
    float pull;
    float shift;
    float mix;
    
    autotalent_lv2* talent = (autotalent_lv2*)instance;

    input = talent->_input_buffer1;
    output = talent->_output_buffer1;
    
    amount = (float)*(talent->_amount);
    smooth = (float)*(talent->_smooth) * 0.8; // Scales max to a more reasonable value
    int scwarp = (int)*(talent->_scwarp);
    
    aref = (float)*(talent->_tune);
    notes[0] = (int)*(talent->_A);
    notes[1] = (int)*(talent->_Bb);
    notes[2] = (int)*(talent->_B);
    notes[3] = (int)*(talent->_C);
    notes[4] = (int)*(talent->_Db);
    notes[5] = (int)*(talent->_D);
    notes[6] = (int)*(talent->_Eb);
    notes[7] = (int)*(talent->_E);
    notes[8] = (int)*(talent->_F);
    notes[9] = (int)*(talent->_Gb);
    notes[10] = (int)*(talent->_G);
    notes[11] = (int)*(talent->_Ab);
    
    fixed = (float)*(talent->_fixed);
    pull = (float)*(talent->_pull);
    shift = (float)*(talent->_shift);
    
    mix = (float)*(talent->_mix);
    
    talent->_talent.set_aref(aref);
    talent->_talent.set_at_fixed(fixed);
    talent->_talent.set_at_shift(shift);
    talent->_talent.set_at_pull(pull);
    talent->_talent.set_at_smooth(smooth);
    talent->_talent.set_at_amount(amount);
    talent->_talent.set_at_scwarp(scwarp);
    talent->_talent.set_mix(mix);
    
    talent->_talent.set_at_note(notes);
    
    talent->_talent.run(input, output, n_samples);
    if (talent->_latency)
    {
        *(talent->_latency) = talent->_talent.get_latency();
    }
    
    if (talent->_pitch)
    {
        *(talent->_pitch) = talent->_talent.get_inpitch();
    }
    
    if (talent->_conf)
    {
        *(talent->_conf) = talent->_talent.get_conf();
    }
}

static const LV2_Descriptor descriptor = {"urn:jeremy.salwen:plugins:autotalent",
                                          instantiate,
                                          connect_port,
                                          activate,
                                          run,
                                          deactivate,
                                          cleanup,
                                          extension_data
                                         };
                                         
// Return the plugin descriptor (there's only one in this file)
LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
    return index == 0 ? &descriptor : NULL;
}
