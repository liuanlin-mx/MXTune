/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginGui.h"
#include <list>
#include "kvbuf.h"

#define KV_KEY_INPITCH 0
#define KV_KEY_INPITCH_PITCH 0
#define KV_KEY_INPITCH_CONF 1
#define KV_KEY_INPITCH_TIME 2

#define KV_KEY_TUNE 1
#define KV_KEY_TUNE_IS_MANUAL 0
#define KV_KEY_TUNE_TIME_START 1
#define KV_KEY_TUNE_TIME_END 2
#define KV_KEY_TUNE_PITCH_START 3
#define KV_KEY_TUNE_PITCH_END 4
#define KV_KEY_TUNE_ATTACK 5
#define KV_KEY_TUNE_RELEASE 6
#define KV_KEY_TUNE_AMOUNT 7

#define KV_KEY_PARAMTERS 2
#define KV_KEY_MISC 3



//==============================================================================
AutotalentAudioProcessor::AutotalentAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    for (std::uint32_t i = 0; i < sizeof(_parameters) / sizeof(_parameters[0]); i++)
    {
        _parameters[i].parameter = new PluginParameter(_parameters[i].name, _parameters[i].def / _parameters[i].scale,
            _parameters[i].min / _parameters[i].scale, _parameters[i].max / _parameters[i].scale, _parameters[i].is_boolean);
        _parameters[i].parameter->addListener(this);
        addParameter(_parameters[i].parameter);
    }
    
    _notes[0] = get_parameter(PARAMETER_ID_A) > 0 ? 1: -1;
    _notes[1] = get_parameter(PARAMETER_ID_Bb) > 0 ? 1: -1;
    _notes[2] = get_parameter(PARAMETER_ID_B) > 0 ? 1: -1;
    _notes[3] = get_parameter(PARAMETER_ID_C) > 0 ? 1: -1;
    _notes[4] = get_parameter(PARAMETER_ID_Db) > 0 ? 1: -1;
    _notes[5] = get_parameter(PARAMETER_ID_D) > 0 ? 1: -1;
    _notes[6] = get_parameter(PARAMETER_ID_Eb) > 0 ? 1: -1;
    _notes[7] = get_parameter(PARAMETER_ID_E) > 0 ? 1: -1;
    _notes[8] = get_parameter(PARAMETER_ID_F) > 0 ? 1: -1;
    _notes[9] = get_parameter(PARAMETER_ID_Gb) > 0 ? 1: -1;
    _notes[10] = get_parameter(PARAMETER_ID_G) > 0 ? 1: -1;
    _notes[11] = get_parameter(PARAMETER_ID_Ab) > 0 ? 1: -1;
    
    _at_amount = get_parameter(PARAMETER_ID_AT_AMOUNT);
    _at_smooth = get_parameter(PARAMETER_ID_AT_SMOOTH);
    _is_enable_at = get_parameter(PARAMETER_ID_ENABLE_AUTOTUNE);
    _is_enable_track = get_parameter(PARAMETER_ID_ENABLE_TRACK);
    _det_alg = get_parameter(PARAMETER_ID_DET_ALG);
    _sft_alg = get_parameter(PARAMETER_ID_SFT_ALG);
    
    _conf_thresh = get_parameter(PARAMETER_ID_VTHRESH);
    
    _misc_param = "st.sequence_ms=16\n"
                    "st.seekwindow_ms=4\n"
                    "st.overlap_ms=4\n";
                    
    _create_mxtune(_sample_rate);
}

AutotalentAudioProcessor::~AutotalentAudioProcessor()
{
    for (std::uint32_t i = 0; i < sizeof(_parameters) / sizeof(_parameters[0]); i++)
    {
        _parameters[i].parameter->removeListener(this);
    }
}

//==============================================================================
const String AutotalentAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AutotalentAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AutotalentAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AutotalentAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AutotalentAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AutotalentAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AutotalentAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AutotalentAudioProcessor::setCurrentProgram (int index)
{
}

const String AutotalentAudioProcessor::getProgramName (int index)
{
    return {};
}

void AutotalentAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AutotalentAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    if (_mx_tune && _sample_rate != (std::uint32_t)sampleRate)
    {
        _sample_rate = (std::uint32_t)sampleRate;
        _mx_tune->set_sample_rate(_sample_rate);
        _report_latency_samples();
    }
}

void AutotalentAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AutotalentAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AutotalentAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    midiMessages.clear();

    if (_is_bypassed)
    {
        _is_bypassed = false;
        _report_latency_samples();
    }
    
    AudioPlayHead *play_head = getPlayHead();
    if (play_head)
    {
        AudioPlayHead::CurrentPositionInfo result;
        if (play_head->getCurrentPosition(result))
        {
            _cur_time = result.timeInSeconds;
            _bpm = result.bpm;
            _ppq_position = result.ppqPosition;
            _time_sig_denominator = result.timeSigDenominator;
            _is_playing = result.isPlaying;
        }
    }
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    if (totalNumInputChannels > 0)
    {
        auto* channel_data = buffer.getWritePointer (0);
        std::int32_t num_samples =  buffer.getNumSamples();
        if (_mx_tune)
        {
            _mx_tune->run(channel_data, channel_data, num_samples, _cur_time);
            
            std::list<mx_tune::midi_msg_node> msg_list = _mx_tune->output_midi_from_note(num_samples, _cur_time);
            for (auto& midi_msg: msg_list)
            {
                if (midi_msg.msg.is_note_on())
                {
                    MidiMessage msg = MidiMessage::noteOn(midi_msg.msg.get_channel(), midi_msg.msg.get_note(), (float)80);
                    midiMessages.addEvent(msg, midi_msg.sample_position);
                }
                else if (midi_msg.msg.is_note_off())
                {
                    MidiMessage msg = MidiMessage::noteOff(midi_msg.msg.get_channel(), midi_msg.msg.get_note(), (float)80);
                    midiMessages.addEvent(msg, midi_msg.sample_position);
                }
            }
        }
        
        for (int channel = 1; channel < totalNumInputChannels; ++channel)
        {
            buffer.copyFrom(channel, 0, channel_data, num_samples);
        }
    }
    
}


void AutotalentAudioProcessor::processBlockBypassed (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    if (!_is_bypassed)
    {
        _is_bypassed = true;
        _report_latency_samples();
    }
    AudioProcessor::processBlockBypassed(buffer, midiMessages);
}

//==============================================================================
bool AutotalentAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AutotalentAudioProcessor::createEditor()
{
    return new PluginGui (*this);
}

//==============================================================================
void AutotalentAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    if (_mx_tune == nullptr)
    {
        return;
    }
    
    kvbuf_hooks hooks = {malloc, free};
    kvbuf *kv_root = kvbuf_create_object(&hooks);
    
    float time_begin = 0;
    float time_end = _mx_tune->get_manual_tune().get_time_len();
    
    {
        std::list<std::pair<manual_tune::pitch_node, float> > inpitch = _mx_tune->get_manual_tune().get_inpitch(time_begin, time_end);
        kvbuf *pitch_array = kvbuf_create_array(&hooks);
        kvbuf *conf_array = kvbuf_create_array(&hooks);
        kvbuf *time_array = kvbuf_create_array(&hooks);
        kvbuf *item = kvbuf_create_object(&hooks);
        for (auto i: inpitch)
        {
            kvbuf_add_item_to_array(pitch_array, kvbuf_create_float32(&hooks, i.first.pitch));
            kvbuf_add_item_to_array(conf_array, kvbuf_create_float32(&hooks, i.first.conf));
            kvbuf_add_item_to_array(time_array, kvbuf_create_float32(&hooks, i.second));
        }
        kvbuf_add_item_to_object(item, KV_KEY_INPITCH_PITCH, pitch_array);
        kvbuf_add_item_to_object(item, KV_KEY_INPITCH_CONF, conf_array);
        kvbuf_add_item_to_object(item, KV_KEY_INPITCH_TIME, time_array);
        
        kvbuf_add_item_to_object(kv_root, KV_KEY_INPITCH, item);
    }
    /*
    {
        std::list<std::pair<manual_tune::pitch_node, float> > outpitch = _mx_tune->get_manual_tune().get_outpitch(time_begin, time_end);
        for (auto i: outpitch)
        {
            
            ReferenceCountedObjectPtr<DynamicObject> item(new DynamicObject);
            item->setProperty("pitch", var(i.first.pitch));
            item->setProperty("conf", var(i.first.conf));
            item->setProperty("time", var(i.second));
            outpitch_arr.add(item.get());
        }
        root->setProperty("outpitch", outpitch_arr);
    }
    */
    
    {
        kvbuf *array = kvbuf_create_array(&hooks);
        std::list<std::shared_ptr<manual_tune::tune_node> > tune = _mx_tune->get_manual_tune().get_tune(time_begin, time_end);
        for (auto i: tune)
        {
            kvbuf *item = kvbuf_create_object(&hooks);
            kvbuf_add_item_to_object(item, KV_KEY_TUNE_IS_MANUAL, kvbuf_create_int8(&hooks, i->is_manual));
            kvbuf_add_item_to_object(item, KV_KEY_TUNE_TIME_START, kvbuf_create_float32(&hooks, i->time_start));
            kvbuf_add_item_to_object(item, KV_KEY_TUNE_TIME_END, kvbuf_create_float32(&hooks, i->time_end));
            kvbuf_add_item_to_object(item, KV_KEY_TUNE_PITCH_START, kvbuf_create_float32(&hooks, i->pitch_start));
            kvbuf_add_item_to_object(item, KV_KEY_TUNE_PITCH_END, kvbuf_create_float32(&hooks, i->pitch_end));
            kvbuf_add_item_to_object(item, KV_KEY_TUNE_ATTACK, kvbuf_create_float32(&hooks, i->attack));
            kvbuf_add_item_to_object(item, KV_KEY_TUNE_RELEASE, kvbuf_create_float32(&hooks, i->release));
            kvbuf_add_item_to_object(item, KV_KEY_TUNE_AMOUNT, kvbuf_create_float32(&hooks, i->amount));
            kvbuf_add_item_to_array(array, item);
        }
        kvbuf_add_item_to_object(kv_root, KV_KEY_TUNE, array);
    }
    
    {
        kvbuf *array = kvbuf_create_array(&hooks);
        for (std::uint32_t i = 0; i < sizeof(_parameters) / sizeof(_parameters[0]); i++)
        {
            kvbuf_add_item_to_array(array, kvbuf_create_float32(&hooks, get_parameter(i)));
        }
        kvbuf_add_item_to_object(kv_root, KV_KEY_PARAMTERS, array);
    }
    
    {
        kvbuf *array = kvbuf_create_array(&hooks);
        for (std::int32_t i = 0; i < _misc_param.length(); i++)
        {
            kvbuf_add_item_to_array(array, kvbuf_create_int8(&hooks, _misc_param.c_str()[i]));
        }
        kvbuf_add_item_to_object(kv_root, KV_KEY_MISC, array);
    }
    
    {
        unsigned char *buf = (unsigned char *)malloc(1024 * 1024);
        std::uint32_t len = kvbuf_build(&hooks, kv_root, buf + 5, 1024 * 1024 - 5);
        kvbuf_delete(&hooks, kv_root);
        memcpy(buf, "kvbuf", 5);
        destData.setSize(len + 5);
        destData.copyFrom(buf, 0, len + 5);
        free(buf);
    }
}

void AutotalentAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    if (sizeInBytes < 1 || _mx_tune == nullptr)
    {
        return;
    }

    const char *first = static_cast<const char *>(data);
    const char *last = first + sizeInBytes - 1;


    _mx_tune->get_manual_tune().disable_history();
    
    if (*first == '{' && *last == '}')
    {
        var root = JSON::fromString(String(first, sizeInBytes));
        if (root.isVoid())
        {
            return;
        }
        
        if (!root.hasProperty("inpitch")
            /*|| !root.hasProperty("outpitch")*/
            || !root.hasProperty("tune"))
        {
            return;
        }
        
        {
            var inpitch = root["inpitch"];
            if (!inpitch.isArray())
            {
                return;
            }
            
            std::int32_t size = inpitch.size();
            if (size > 0
                && inpitch[0].hasProperty("pitch")
                && inpitch[0].hasProperty("conf")
                && inpitch[0].hasProperty("time"))
            {
                manual_tune::pitch_node last_pitch;
                last_pitch.pitch = inpitch[0]["pitch"];
                last_pitch.conf = inpitch[0]["conf"];
                float last_time = inpitch[0]["time"];
                for (std::int32_t i = 1; i < size; i++)
                {
                    if (inpitch[i].hasProperty("pitch")
                        && inpitch[i].hasProperty("conf")
                        && inpitch[i].hasProperty("time"))
                    {
                        manual_tune::pitch_node pitch;
                        pitch.pitch = inpitch[i]["pitch"];
                        pitch.conf = inpitch[i]["conf"];
                        float time = inpitch[i]["time"];
                        
                        _mx_tune->get_manual_tune().set_inpitch(last_time, time, last_pitch);
                        last_pitch = pitch;
                        last_time = time;
                    }
                }
            }
        }
        
        /*
        {
            var outpitch = root["outpitch"];
            if (!outpitch.isArray())
            {
                return;
            }
            
            std::int32_t size = outpitch.size();
            if (size > 0
                && outpitch[0].hasProperty("pitch")
                && outpitch[0].hasProperty("conf")
                && outpitch[0].hasProperty("time"))
            {
                manual_tune::pitch_node last_pitch;
                last_pitch.pitch = outpitch[0]["pitch"];
                last_pitch.conf = outpitch[0]["conf"];
                float last_time = outpitch[0]["time"];
                for (std::int32_t i = 1; i < size; i++)
                {
                    if (outpitch[i].hasProperty("pitch")
                        && outpitch[i].hasProperty("conf")
                        && outpitch[i].hasProperty("time"))
                    {
                        manual_tune::pitch_node pitch;
                        pitch.pitch = outpitch[i]["pitch"];
                        pitch.conf = outpitch[i]["conf"];
                        float time = outpitch[i]["time"];
                        
                        _mx_tune->get_manual_tune().set_outpitch(last_time, time, last_pitch);
                        last_pitch = pitch;
                        last_time = time;
                    }
                }
            }
        }
        */
        
        {
            var tune = root["tune"];
            if (!tune.isArray())
            {
                return;
            }
            
            std::int32_t size = tune.size();
            for (std::int32_t i = 0; i < size; i++)
            {
                std::shared_ptr<manual_tune::tune_node> node(new manual_tune::tune_node);
                
                if (tune[i].hasProperty("is_manual"))
                {
                    node->is_manual = tune[i]["is_manual"];
                }
                
                if (tune[i].hasProperty("time_start")
                    && tune[i].hasProperty("time_end")
                    && tune[i].hasProperty("pitch_start")
                    && tune[i].hasProperty("pitch_end")
                    && tune[i].hasProperty("attack")
                    && tune[i].hasProperty("release")
                    && tune[i].hasProperty("amount"))
                {
                    node->time_start = tune[i]["time_start"];
                    node->time_end = tune[i]["time_end"];
                    node->pitch_start = tune[i]["pitch_start"];
                    node->pitch_end = tune[i]["pitch_end"];
                    node->attack = tune[i]["attack"];
                    node->release = tune[i]["release"];
                    node->amount = tune[i]["amount"];
                    
                    _mx_tune->get_manual_tune().add_tune(node);
                }
            }
        }
        
        if (root.hasProperty("paramters"))
        {
            var paramters = root["paramters"];
            if (!paramters.isArray())
            {
                return;
            }
            
            std::int32_t size = paramters.size();
            for (std::int32_t i = 0; i < size; i++)
            {
                set_parameter(i, paramters[i]);
            }
        }

        if (root.hasProperty("misc"))
        {
            var misc = root["misc"];
            if (!misc.isString())
            {
                return;
            }
            _misc_param = misc.toString().toStdString();
            _mx_tune->set_misc_param(_misc_param);
            _report_latency_samples();
        }
    }
    else if (sizeInBytes > 5 && memcmp(first, "kvbuf", 5) == 0)
    {
        kvbuf_hooks hooks = {malloc, free};
        kvbuf *kv_root = kvbuf_parse(&hooks, (const std::uint8_t *)first + 5, (const std::uint8_t *)first + sizeInBytes);
        if (kv_root == NULL)
        {
            return;
        }
        
        kvbuf *inpitch = kvbuf_get_object_item(kv_root, KV_KEY_INPITCH);
        kvbuf *tune = kvbuf_get_object_item(kv_root, KV_KEY_TUNE);
        if (!inpitch || !tune)
        {
            kvbuf_free(&hooks, kv_root);
            return;
        }
        
        {
            kvbuf *pitch_array = kvbuf_get_object_item(inpitch, KV_KEY_INPITCH_PITCH);
            kvbuf *conf_array = kvbuf_get_object_item(inpitch, KV_KEY_INPITCH_CONF);
            kvbuf *time_array = kvbuf_get_object_item(inpitch, KV_KEY_INPITCH_TIME);
            
            if (!pitch_array || !conf_array || !time_array)
            {
                kvbuf_free(&hooks, kv_root);
                return;
            }
            
            
            float last_time = 0;
            manual_tune::pitch_node last_pitch;
            
            kvbuf *pitch_it = kvbuf_get_array_begin(pitch_array);
            kvbuf *conf_it = kvbuf_get_array_begin(conf_array);
            kvbuf *time_it = kvbuf_get_array_begin(time_array);
            
            if (pitch_it && conf_it && time_it)
            {
                last_pitch.pitch = kvbuf_get_float32(pitch_it);
                last_pitch.conf = kvbuf_get_float32(conf_it);
                last_time = kvbuf_get_float32(time_it);
                
                pitch_it = kvbuf_get_array_next(pitch_it);
                conf_it = kvbuf_get_array_next(conf_it);
                time_it = kvbuf_get_array_next(time_it);
            }
            
            while (pitch_it && conf_it && time_it)
            {
                manual_tune::pitch_node pitch;
                pitch.pitch = kvbuf_get_float32(pitch_it);
                pitch.conf = kvbuf_get_float32(conf_it);
                float time = kvbuf_get_float32(time_it);
                        
                _mx_tune->get_manual_tune().set_inpitch(last_time, time, last_pitch);
                last_pitch = pitch;
                last_time = time;
                
                pitch_it = kvbuf_get_array_next(pitch_it);
                conf_it = kvbuf_get_array_next(conf_it);
                time_it = kvbuf_get_array_next(time_it);
            }
        }
        
        {
            if (!kvbuf_is_array(tune))
            {
                kvbuf_free(&hooks, kv_root);
                return;
            }
            
            kvbuf *item = kvbuf_get_array_begin(tune);
            while (item)
            {
                std::shared_ptr<manual_tune::tune_node> node(new manual_tune::tune_node);
                
                node->is_manual = kvbuf_get_int8(kvbuf_get_object_item(item, KV_KEY_TUNE_IS_MANUAL));
                node->time_start = kvbuf_get_float32(kvbuf_get_object_item(item, KV_KEY_TUNE_TIME_START));
                node->time_end = kvbuf_get_float32(kvbuf_get_object_item(item, KV_KEY_TUNE_TIME_END));
                node->pitch_start = kvbuf_get_float32(kvbuf_get_object_item(item, KV_KEY_TUNE_PITCH_START));
                node->pitch_end = kvbuf_get_float32(kvbuf_get_object_item(item, KV_KEY_TUNE_PITCH_END));
                node->attack = kvbuf_get_float32(kvbuf_get_object_item(item, KV_KEY_TUNE_ATTACK));
                node->release = kvbuf_get_float32(kvbuf_get_object_item(item, KV_KEY_TUNE_RELEASE));
                node->amount = kvbuf_get_float32(kvbuf_get_object_item(item, KV_KEY_TUNE_AMOUNT));
                
                item = kvbuf_get_array_next(item);
                
                _mx_tune->get_manual_tune().add_tune(node);
            }
        }



        kvbuf *paramters = kvbuf_get_object_item(kv_root, KV_KEY_PARAMTERS);
        if (paramters)
        {
            if (!kvbuf_is_array(paramters))
            {
                kvbuf_free(&hooks, kv_root);
                return;
            }
            
            std::int32_t size = kvbuf_get_array_size(paramters);
            for (std::int32_t i = 0; i < size; i++)
            {
                set_parameter(i, kvbuf_get_float32(kvbuf_get_array_item(paramters, i)));
            }
        }
            
        kvbuf *misc = kvbuf_get_object_item(kv_root, KV_KEY_MISC);
        if (misc)
        {
            if (!kvbuf_is_array(misc))
            {
                kvbuf_free(&hooks, kv_root);
                return;
            }
            _misc_param.clear();
            std::int32_t size = kvbuf_get_array_size(misc);
            for (std::int32_t i = 0; i < size; i++)
            {
                _misc_param.push_back(kvbuf_get_int8(kvbuf_get_array_item(misc, i)));
            }
            _mx_tune->set_misc_param(_misc_param);
            _report_latency_samples();
        }
        kvbuf_free(&hooks, kv_root);
    }
    
    _mx_tune->get_manual_tune().enable_history();
}


void AutotalentAudioProcessor::parameterValueChanged (int parameterIndex, float newValue)
{
    std::lock_guard<std::mutex> l(_mtx);

    if (parameterIndex >= PARAMETER_ID_A && parameterIndex <= PARAMETER_ID_Ab)
    {
        _notes[parameterIndex - PARAMETER_ID_A] = (newValue > 0)? 1: -1;
        if (_mx_tune)
        {
            _mx_tune->set_at_note(_notes);
        }
    }
    else if (parameterIndex == PARAMETER_ID_AT_AMOUNT)
    {
        _at_amount = newValue;
        if (_mx_tune)
        {
            _mx_tune->set_at_amount(newValue);
        }
    }
    else if (parameterIndex == PARAMETER_ID_AT_SMOOTH)
    {
        _at_smooth = newValue;
        if (_mx_tune)
        {
            _mx_tune->set_at_smooth(newValue);
        }
    }
    else if (parameterIndex == PARAMETER_ID_ENABLE_AUTOTUNE)
    {
        _is_enable_at = newValue > 0.;
        if (_mx_tune)
        {
            _mx_tune->enable_auto_tune(_is_enable_at);
        }
    }
    else if (parameterIndex == PARAMETER_ID_ENABLE_TRACK)
    {
        _is_enable_track = newValue > 0.;
        if (_mx_tune)
        {
            _mx_tune->enable_track(_is_enable_track);
        }
    }
    else if (parameterIndex == PARAMETER_ID_DET_ALG)
    {
        _det_alg = round(newValue * _parameters[parameterIndex].scale);
        if (_mx_tune)
        {
            _mx_tune->set_detector(_det_alg);
        }
    }
    else if (parameterIndex == PARAMETER_ID_SFT_ALG)
    {
        _sft_alg = round(newValue * _parameters[parameterIndex].scale);
        if (_mx_tune)
        {
            _mx_tune->set_shifter(_sft_alg);
            _report_latency_samples();
        }
    }
    else if (parameterIndex == PARAMETER_ID_VTHRESH)
    {
        _conf_thresh = newValue * _parameters[parameterIndex].scale;
        if (_mx_tune)
        {
            _mx_tune->set_conf_shift_thresh(_conf_thresh);
            _mx_tune->set_conf_detect_thresh((_conf_thresh > 0.2)? (_conf_thresh - 0.2): 0.1);
        }
    }
    else if (parameterIndex == PARAMETER_ID_AFREQ)
    {
        _afreq = newValue * _parameters[parameterIndex].scale;
        if (_mx_tune)
        {
            _mx_tune->set_aref(_afreq);
        }
    }
    else if (parameterIndex == PARAMETER_ID_DET_GATE)
    {
        _det_gate = newValue * _parameters[parameterIndex].scale;
        if (_mx_tune)
        {
            _mx_tune->set_detect_gate(-_det_gate);
        }
    }
    else if (parameterIndex == PARAMETER_ID_DET_MIN_FREQ)
    {
        _det_min_freq = newValue * _parameters[parameterIndex].scale;
        if (_mx_tune)
        {
            _mx_tune->set_detect_freq_range(_det_min_freq, _det_max_freq);
        }
    }
    else if (parameterIndex == PARAMETER_ID_DET_MAX_FREQ)
    {
        _det_max_freq = newValue * _parameters[parameterIndex].scale;
        if (_mx_tune)
        {
            _mx_tune->set_detect_freq_range(_det_min_freq, _det_max_freq);
        }
    }
    
    if (!_gesture_is_starting)
    {
        _parameter_update_id++;
    }
    
}

void AutotalentAudioProcessor::parameterGestureChanged (int parameterIndex, bool gestureIsStarting)
{
    //std::lock_guard<std::mutex> l(_mtx);
    _gesture_is_starting = gestureIsStarting;
}


float AutotalentAudioProcessor::get_parameter(std::uint32_t id)
{
    if (id < PARAMETER_ID_NUM)
    {
        return _parameters[id].parameter->getValue() * _parameters[id].scale;
    }
    return 0.;
}

void AutotalentAudioProcessor::set_parameter(std::uint32_t id, float v)
{
    if (id < PARAMETER_ID_NUM)
    {
        if (v < _parameters[id].min)
        {
            v = _parameters[id].min;
        }
        if (v > _parameters[id].max)
        {
            v = _parameters[id].max;
        }
        v = v / _parameters[id].scale;
        if (v > 1.0)
        {
            v = 1.0;
        }
        _parameters[id].parameter->beginChangeGesture();
        _parameters[id].parameter->setValueNotifyingHost(v);
        _parameters[id].parameter->endChangeGesture();
    }
}


void AutotalentAudioProcessor::set_misc_param(const std::string& misc_param)
{
    _misc_param = misc_param;
    if (_mx_tune)
    {
        _mx_tune->set_misc_param(_misc_param);
    }
}

void AutotalentAudioProcessor::_create_mxtune(std::uint32_t sample_rate)
{
    _mx_tune.reset(new (std::nothrow) mx_tune(sample_rate));
    if (_mx_tune)
    {
        _mx_tune->set_at_note(_notes);
        _mx_tune->set_at_amount(_at_amount);
        _mx_tune->set_at_smooth(_at_smooth);
        _mx_tune->enable_auto_tune(_is_enable_at);
        _mx_tune->enable_track(_is_enable_track);
        _mx_tune->set_detector(_det_alg);
        _mx_tune->set_shifter(_sft_alg);
        _mx_tune->set_conf_shift_thresh(_conf_thresh);
        _mx_tune->set_conf_detect_thresh((_conf_thresh > 0.2)? (_conf_thresh - 0.2): 0.1);
        _mx_tune->set_aref(_afreq);
        _mx_tune->set_detect_gate(-_det_gate);
        _mx_tune->set_detect_freq_range(_det_min_freq, _det_max_freq);
        _mx_tune->set_misc_param(_misc_param);
        setLatencySamples(_mx_tune->get_latency());
    }
}


void AutotalentAudioProcessor::_report_latency_samples()
{
    if (_is_bypassed)
    {
        setLatencySamples(0);
    }
    else
    {
        setLatencySamples(_mx_tune->get_latency());
    }
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AutotalentAudioProcessor();
}
