/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <mutex>
#include <JuceHeader.h>
#include "mx_tune.h"
#include "PluginParameter.h"

//==============================================================================
/**
*/
class AutotalentAudioProcessor  : public AudioProcessor, public AudioProcessorParameter::Listener
{
public:
    
    enum
    {
        PARAMETER_ID_SNAP = 0,
        PARAMETER_ID_DEF_ATTACK,
        PARAMETER_ID_DEF_RELEASE,
        PARAMETER_ID_DEF_AMOUNT,
        PARAMETER_ID_AT_AMOUNT,
        PARAMETER_ID_AT_SMOOTH,
        PARAMETER_ID_ENABLE_AUTOTUNE,
        PARAMETER_ID_ENABLE_TRACK,
        PARAMETER_ID_KEY,
        PARAMETER_ID_KEY_TYPE,
        PARAMETER_ID_A,
        PARAMETER_ID_Bb,
        PARAMETER_ID_B,
        PARAMETER_ID_C,
        PARAMETER_ID_Db,
        PARAMETER_ID_D,
        PARAMETER_ID_Eb,
        PARAMETER_ID_E,
        PARAMETER_ID_F,
        PARAMETER_ID_Gb,
        PARAMETER_ID_G,
        PARAMETER_ID_Ab,
        PARAMETER_ID_DET_ALG,
        PARAMETER_ID_NUM,
    };
    
    struct parameter_item
    {
        const char *name;
        float def;
        bool is_boolean;
        float scale;
        float min;
        float max;
        PluginParameter *parameter;
    };
public:
    //==============================================================================
    AutotalentAudioProcessor();
    ~AutotalentAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    void parameterValueChanged (int parameterIndex, float newValue) override;
    void parameterGestureChanged (int parameterIndex, bool gestureIsStarting) override;
    
public:
    const std::shared_ptr<mx_tune>& get_autotalent() { return _talent; }
    manual_tune& get_manual_tune() { return _talent->get_manual_tune(); }
    double get_cur_time() { return _cur_time; }
    bool is_playing() { return _is_playing; }
    
    
    float get_parameter(std::uint32_t id);
    void set_parameter(std::uint32_t id, float v);
    std::uint32_t get_parameters_update_id() { return _parameter_update_id; }

    
private:
    std::mutex _mtx;
    std::shared_ptr<mx_tune> _talent = 0;
    double _cur_time = 0;
    bool _is_playing = false;
    
    std::int32_t _notes[12] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    //float _def_attack = 0.050;
    //float _def_release = 0.050;
    //float _def_amount = 0.5;
    
    float _at_amount = 0.6;
    float _at_smooth = 0.7;
    bool _is_enable_at = false;
    bool _is_enable_track = true;
    std::uint32_t _det_alg = 0;
    
    parameter_item _parameters[PARAMETER_ID_NUM] = {
        {"Snap", 1.0, true, 1.0, 0., 1., NULL},
        {"Default Attack", 0.050, false, 1., 0., 0.5, NULL},
        {"Default Release", 0.050, false, 1., 0., 0.5, NULL},
        {"Default Amount", 0.5, false, 1., 0., 1., NULL},
        {"Auto Tune Amount", 0.6, false, 1., 0., 1., NULL},
        {"Auto Tune Smooth", 0.7, false, 1., 0., 1., NULL},
        
        {"Enable AutoTune", 1, true, 1., 0., 1., NULL},
        {"Enable Track", 0, false, 1., 0., 1., NULL},
        
        {"Key", 3., false, 100., 0., 12, NULL},
        {"Key Type", 2., false, 10., 0., 3, NULL},
        
        {"A", 1., false, 1., 0., 1., NULL},
        {"Bb", 1., false, 1., 0., 1., NULL},
        {"B", 1., false, 1., 0., 1., NULL},
        {"C", 1., false, 1., 0., 1., NULL},
        {"Db", 1., false, 1., 0., 1., NULL},
        {"D", 1., false, 1., 0., 1., NULL},
        {"Eb", 1., false, 1., 0., 1., NULL},
        {"E", 1., false, 1., 0., 1., NULL},
        {"F", 1., false, 1., 0., 1., NULL},
        {"Gb", 1., false, 1., 0., 1., NULL},
        {"G", 1., false, 1., 0., 1., NULL},
        {"Ab", 1., false, 1., 0., 1., NULL},
        {"Detection algorithm ", 0., false, 10., 0., 1., NULL},
    };
    
    volatile std::uint32_t _parameter_update_id = 0;
    bool _gesture_is_starting = false;
    
    String _state_info;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AutotalentAudioProcessor)
};
