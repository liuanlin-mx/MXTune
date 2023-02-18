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
        PARAMETER_ID_SFT_ALG,
        PARAMETER_ID_VTHRESH,
        PARAMETER_ID_SNAP_MIN_LENGHT,
        PARAMETER_ID_SNAP_MAX_INTERVAL,
        PARAMETER_ID_AFREQ,
        PARAMETER_ID_DET_GATE,
        PARAMETER_ID_DET_MIN_FREQ,
        PARAMETER_ID_DET_MAX_FREQ,
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
    void processBlockBypassed (AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;

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
    const std::shared_ptr<mx_tune>& get_mt_tune() { return _mx_tune; }
    manual_tune& get_manual_tune() { return _mx_tune->get_manual_tune(); }
    double get_cur_time() { return _cur_time; }
    double get_bpm() { return _bpm; }
    double get_ppq_position() { return _ppq_position; }
    std::int32_t get_time_sig_denominator() { return _time_sig_denominator; }
    bool is_playing() { return _is_playing; }
    
    
    float get_parameter(std::uint32_t id);
    void set_parameter(std::uint32_t id, float v);
    std::uint32_t get_parameters_update_id() { return _parameter_update_id; }
    std::string get_misc_param() { return _misc_param; }
    void set_misc_param(const std::string& misc_param);
    void report_latency_samples() { _report_latency_samples(); }
    
private:
    void _create_mxtune(std::uint32_t sample_rate);
    void _report_latency_samples();
    void _record_midi_to_note(MidiBuffer& midiMessages, std::int32_t num_samples, float timestamp);
    void _output_midi_from_note(MidiBuffer& midiMessages, std::int32_t num_samples, float timestamp);
    void _apply_misc_param();
    
private:
    std::mutex _mtx;
    std::shared_ptr<mx_tune> _mx_tune = 0;
    std::uint32_t _sample_rate = 44100;
    double _cur_time = 0;
    double _bpm = 0;
    double _ppq_position = 0;
    std::int32_t _time_sig_denominator = 0;
    bool _is_playing = false;
    bool _is_bypassed = false;
    
    std::int32_t _notes[12] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    //float _def_attack = 0.050;
    //float _def_release = 0.050;
    //float _def_amount = 0.5;
    
    float _at_amount = 0.6;
    float _at_smooth = 0.7;
    bool _is_enable_at = false;
    bool _is_enable_track = true;
    std::uint32_t _det_alg = 0;
    std::uint32_t _sft_alg = 0;
    float _conf_thresh = 0.9;
    float _afreq = 440;
    float _det_gate = 60;
    float _det_min_freq = 70;
    float _det_max_freq = 800;
    bool _midi_record = false;
    bool _midi_export = false;
    
    /* name def is_boolean scale min max parameter */
    parameter_item _parameters[PARAMETER_ID_NUM] = {
        {"Snap", 1.0, true, 1.0, 0., 1., NULL},
        {"Default Attack", 0.02, false, 1., 0., 0.5, NULL},
        {"Default Release", 0.02, false, 1., 0., 0.5, NULL},
        {"Default Amount", 0.7, false, 1., 0., 1., NULL},
        {"Auto Tune Amount", 0.8, false, 1., 0., 1., NULL},
        {"Auto Tune Smooth", 0.7, false, 1., 0., 1., NULL},
        
        {"Enable AutoTune", 0, true, 1., 0., 1., NULL},
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
        {"detection algorithm ", 1., false, 10., 0., 1., NULL},
        {"shifter algorithm ", 0., false, 10., 0., 3., NULL},
        {"conf thresh", 0.9, false, 1., 0., 1., NULL},
        {"snap min length", 0.1, false, 1., 0., 1., NULL},
        {"snap max interval", 0.02, false, 1., 0., 1., NULL},
        {"afreq", 440, false, 1000., 430., 450., NULL},
        {"detector gate", 60, false, 200., 10., 100., NULL},
        {"detector min freq", 70, false, 3000., 20., 2000., NULL},
        {"detector max freq", 800., false, 3000., 20., 2000., NULL},
        /* name def is_boolean scale min max parameter */
    };
    
    std::string _misc_param;
    volatile std::uint32_t _parameter_update_id = 0;
    bool _gesture_is_starting = false;
    
    String _state_info;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AutotalentAudioProcessor)
};
