/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Repeat.h"
#include <cmath>

//==============================================================================
/**
*/
class REPEATAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    REPEATAudioProcessor();
    ~REPEATAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    std::vector<Repeat> repeat;
    bool isPlaying = false;
    
    juce::AudioProcessorValueTreeState parameters;
    juce::AudioProcessorValueTreeState::Listener* listener;

private:

    std::atomic<float>* intervalParameter = nullptr;
    
    class ParameterListener : public juce::AudioProcessorValueTreeState::Listener {
    public:
        ParameterListener(REPEATAudioProcessor& _p) : p(_p){}
        void parameterChanged(const juce::String& parameterID, float newValue) override {
           /* if (parameterID == "INTERVAL")*/
            
            int rounded = juce::roundToInt(newValue);
            for (int i = 0; i < p.repeat.size(); i++) {
                if (rounded == 0) p.repeat[i].setInterval(0);
                else p.repeat[i].setInterval(std::pow(2, rounded - 1));
                
            }
        }
    private:
        REPEATAudioProcessor& p;
    };
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (REPEATAudioProcessor)
};
