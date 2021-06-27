/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginGui.h"
#include <list>

//==============================================================================
AutotalentAudioProcessor::AutotalentAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::mono(), true)
                     #endif
                       )
#endif
{
    for (std::int32_t i = 0; i < sizeof(_parameters) / sizeof(_parameters[0]); i++)
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
}

AutotalentAudioProcessor::~AutotalentAudioProcessor()
{
    for (std::int32_t i = 0; i < sizeof(_parameters) / sizeof(_parameters[0]); i++)
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
    
    if (_talent == nullptr)
    {
        _talent.reset(new mx_tune(sampleRate));
        if (_talent)
        {
            setLatencySamples(_talent->get_latency());
            _talent->set_at_note(_notes);
            _talent->set_at_amount(_at_amount);
            _talent->set_at_smooth(_at_smooth);
            _talent->enable_auto_tune(_is_enable_at);
            _talent->enable_track(_is_enable_track);
        }
    }
}

void AutotalentAudioProcessor::releaseResources()
{
    if (_talent)
    {
        _talent.reset();
    }
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


    AudioPlayHead *play_head = getPlayHead();
    if (play_head)
    {
        AudioPlayHead::CurrentPositionInfo result;
        if (play_head->getCurrentPosition(result))
        {
            _cur_pos = result.timeInSeconds;
            _is_playing = result.isPlaying;
        }
    }
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        if (_talent)
        {
            _talent->run(channelData, channelData, buffer.getNumSamples(), _cur_pos);
        }
        // ..do something to the data...
    }
    
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
    
    if (_talent == nullptr)
    {
        return;
    }
    
    ReferenceCountedObjectPtr<DynamicObject> root(new DynamicObject);
    
    float time_begin = 0;
    float time_end = _talent->get_manual_tune().get_time_len();
    Array<var> inpitch_arr;
    Array<var> outpitch_arr;
    Array<var> tune_arr;
    
    {
        std::list<std::pair<manual_tune::pitch_node, float> > inpitch = _talent->get_manual_tune().get_inpitch(time_begin, time_end);
        for (auto i: inpitch)
        {
            ReferenceCountedObjectPtr<DynamicObject> item(new DynamicObject);
            item->setProperty("pitch", var(i.first.pitch));
            item->setProperty("conf", var(i.first.conf));
            item->setProperty("time", var(i.second));
            inpitch_arr.add(item.get());
        }
        root->setProperty("inpitch", inpitch_arr);
    }
    {
        std::list<std::pair<manual_tune::pitch_node, float> > outpitch = _talent->get_manual_tune().get_outpitch(time_begin, time_end);
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
    
    {
        std::list<std::shared_ptr<manual_tune::tune_node> > tune = _talent->get_manual_tune().get_tune(time_begin, time_end);
        for (auto i: tune)
        {
            ReferenceCountedObjectPtr<DynamicObject> item(new DynamicObject);
            item->setProperty("time_start", var(i->time_start));
            item->setProperty("time_end", var(i->time_end));
            item->setProperty("pitch_start", var(i->pitch_start));
            item->setProperty("pitch_end", var(i->pitch_end));
            item->setProperty("attack", var(i->attack));
            item->setProperty("release", var(i->release));
            item->setProperty("amount", var(i->amount));
            tune_arr.add(item.get());
        }
        root->setProperty("tune", tune_arr);
    }
    String s = JSON::toString(root.get());
    destData.setSize(s.length());
    destData.copyFrom(s.getCharPointer(), 0, destData.getSize());
}

void AutotalentAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    if (sizeInBytes < 1 || _talent == nullptr)
    {
        return;
    }

    const char *first = static_cast<const char *>(data);
    const char *last = first + sizeInBytes - 1;

    if (*first != '{' || *last != '}')
    {
        return;
    }
    
    var root = JSON::fromString(String(first, sizeInBytes));
    if (root.isVoid())
    {
        return;
    }
    
    if (!root.hasProperty("inpitch")
        || !root.hasProperty("outpitch")
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
                    
                    _talent->get_manual_tune().set_inpitch(last_time, time, last_pitch);
                    last_pitch = pitch;
                    last_time = time;
                }
            }
        }
    }
    
    
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
                    
                    _talent->get_manual_tune().set_outpitch(last_time, time, last_pitch);
                    last_pitch = pitch;
                    last_time = time;
                }
            }
        }
    }
    
    
    {
        var tune = root["tune"];
        if (!tune.isArray())
        {
            return;
        }
        
        std::int32_t size = tune.size();
        if (size > 0)
        {
            for (std::int32_t i = 0; i < size; i++)
            {
                if (tune[i].hasProperty("time_start")
                    && tune[i].hasProperty("time_end")
                    && tune[i].hasProperty("pitch_start")
                    && tune[i].hasProperty("pitch_end")
                    && tune[i].hasProperty("attack")
                    && tune[i].hasProperty("release")
                    && tune[i].hasProperty("amount"))
                {
                    std::shared_ptr<manual_tune::tune_node> node(new manual_tune::tune_node);
                    node->time_start = tune[i]["time_start"];
                    node->time_end = tune[i]["time_end"];
                    node->pitch_start = tune[i]["pitch_start"];
                    node->pitch_end = tune[i]["pitch_end"];
                    node->attack = tune[i]["attack"];
                    node->release = tune[i]["release"];
                    node->amount = tune[i]["amount"];
                    
                    _talent->get_manual_tune().set_tune(node);
                }
            }
        }
    }

}


void AutotalentAudioProcessor::parameterValueChanged (int parameterIndex, float newValue)
{
    std::lock_guard<std::mutex> l(_mtx);

    if (parameterIndex >= PARAMETER_ID_A && parameterIndex <= PARAMETER_ID_Ab)
    {
        _notes[parameterIndex - PARAMETER_ID_A] = (newValue > 0)? 1: -1;
        if (_talent)
        {
            _talent->set_at_note(_notes);
        }
    }
    else if (parameterIndex == PARAMETER_ID_AT_AMOUNT)
    {
        _at_amount = newValue;
        if (_talent)
        {
            _talent->set_at_amount(newValue);
        }
    }
    else if (parameterIndex == PARAMETER_ID_AT_SMOOTH)
    {
        _at_smooth = newValue;
        if (_talent)
        {
            _talent->set_at_smooth(newValue);
        }
    }
    else if (parameterIndex == PARAMETER_ID_ENABLE_AUTOTUNE)
    {
        _is_enable_at = newValue > 0.;
        if (_talent)
        {
            _talent->enable_auto_tune(_is_enable_at);
        }
    }
    else if (parameterIndex == PARAMETER_ID_ENABLE_TRACK)
    {
        _is_enable_track = newValue > 0.;
        if (_talent)
        {
            _talent->enable_track(_is_enable_track);
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


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AutotalentAudioProcessor();
}
