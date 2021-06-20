/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginGui.h"
#include "net_log.h"

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
    _talent = NULL;
}

AutotalentAudioProcessor::~AutotalentAudioProcessor()
{
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
    net_log_debug("\n");
    if (_talent == nullptr)
    {
        _talent.reset(new autotalent(sampleRate));
        if (_talent)
        {
            setLatencySamples(_talent->get_latency());
        }
    }
}

void AutotalentAudioProcessor::releaseResources()
{
    net_log_debug("\n");
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
    //net_log_debug("totalNumInputChannels:%d totalNumOutputChannels:%d\n", totalNumInputChannels, totalNumOutputChannels);
    
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
        //net_log_debug("isPlaying:%d timeInSeconds:%f\n", result.isPlaying, result.timeInSeconds);
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
}

void AutotalentAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AutotalentAudioProcessor();
}
