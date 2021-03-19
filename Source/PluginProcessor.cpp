/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "Editor.h"

//==============================================================================
REPEATAudioProcessor::REPEATAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
        juce::AudioParameterInt* intervalParameter = new juce::AudioParameterInt(TRANS("INTERVAL"), TRANS("INTERVAL"), 0, 6, 0, juce::String(),
            [] (int value, int maximumStringLength) { 
                if (value == 1) return "1/1";
                else if (value == 2) return "1/2";
                else if (value == 3) return "1/4";
                else if (value == 4) return "1/8";
                else if (value == 5) return "1/16";
                else if (value == 6) return "1/32";
                else return "none";

            },
            [] (const juce::String& text) {
                if (text == "1/1") return 1;
                else if (text == "1/2") return 2;
                else if (text == "1/4") return 3;
                else if (text == "1/8") return 4;
                else if (text == "1/16") return 5;
                else if (text == "1/32") return 6;
                else return 0;
            });
        addParameter(intervalParameter);

}

REPEATAudioProcessor::~REPEATAudioProcessor()
{
}

//==============================================================================
const juce::String REPEATAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool REPEATAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool REPEATAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool REPEATAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double REPEATAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int REPEATAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int REPEATAudioProcessor::getCurrentProgram()
{
    return 0;
}

void REPEATAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String REPEATAudioProcessor::getProgramName (int index)
{
    return {};
}

void REPEATAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void REPEATAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void REPEATAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool REPEATAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void REPEATAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    
    juce::AudioPlayHead::CurrentPositionInfo info;
    getPlayHead()->getCurrentPosition(info);
    auto sampleRate = getSampleRate();
    auto bpm = info.bpm;
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    if (repeat.size() != totalNumInputChannels) {
        repeat.resize(totalNumInputChannels);
        for (int i = 0; i < repeat.size(); i++) {
            repeat[i].clear();
        }
    }
    
    // stop -> setInterval(0)
    isPlaying = info.isPlaying;
    
    const int bufferSize = buffer.getNumSamples();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        repeat[channel].setParameter(sampleRate, bpm);

        for (int i = 0; i < bufferSize; i++) {
            repeat[channel].process(channelData[i]);

        }
    }
}

//==============================================================================
bool REPEATAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* REPEATAudioProcessor::createEditor()
{
    return new Editor (*this);
}

//==============================================================================
void REPEATAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{

}

void REPEATAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

               
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new REPEATAudioProcessor();
}
