/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "PluginProcessor.h"

//[/Headers]

#include "MomentaryRadioButton.h"


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Editor  : public juce::AudioProcessorEditor,
                private juce::Timer
{
public:
    //==============================================================================
    Editor (REPEATAudioProcessor& p);
    ~Editor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;

    // Binary resources:
    static const char* bg_png;
    static const int bg_pngSize;
    static const char* largeButton_1_1_png;
    static const int largeButton_1_1_pngSize;
    static const char* largeButton_1_2_png;
    static const int largeButton_1_2_pngSize;
    static const char* largeButton_1_4_png;
    static const int largeButton_1_4_pngSize;
    static const char* largeButton_1_8_png;
    static const int largeButton_1_8_pngSize;
    static const char* largeButton_1_16_png;
    static const int largeButton_1_16_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void timerCallback() override;
    REPEATAudioProcessor& processor;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<MomentaryRadioButton> momentaryRadioButton;
    juce::Image cachedImage_bg_png_1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

