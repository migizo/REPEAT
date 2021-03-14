/*
  ==============================================================================

    MomentaryRadioButton.h
    Created: 12 Mar 2021 2:11:51pm
    Author:  migizo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ButtonResource.h"
class MomentaryRadioButton : public juce::Component, private juce::Timer{
  
public:
    
    MomentaryRadioButton() {
        cachedImage[0] = juce::ImageCache::getFromMemory (largeButton_1_1_png, largeButton_1_1_pngSize);
        cachedImage[1] = juce::ImageCache::getFromMemory (largeButton_1_2_png, largeButton_1_2_pngSize);
        cachedImage[2] = juce::ImageCache::getFromMemory (largeButton_1_4_png, largeButton_1_4_pngSize);
        cachedImage[3] = juce::ImageCache::getFromMemory (largeButton_1_8_png, largeButton_1_8_pngSize);
        cachedImage[4] = juce::ImageCache::getFromMemory (largeButton_1_16_png, largeButton_1_16_pngSize);
        startTimer(40);
    }
    
    void mouseMove(const juce::MouseEvent& event) override {
        float mouseX = event.position.x;
        state = -1;
        for (int i = 0; i < 5; i++) {
            mouseState[i] = mouseNormal;

            if (buttonSize * i <= mouseX && mouseX < buttonSize * (i + 1)) {
                mouseState[i] = mouseOver;
            }
        }
    }
    
    void mouseDrag(const juce::MouseEvent& event) override {
        float mouseX = event.position.x;
        state = -1;
        for (int i = 0; i < 5; i++) {
            mouseState[i] = mouseNormal;

            if (buttonSize * i <= mouseX && mouseX < buttonSize * (i + 1)) {
                mouseState[i] = mouseDown;
                state = i;
            }
        }
    }
    void mouseExit(const juce::MouseEvent& event) override {
        for (int i = 0; i < 5; i++) {
            mouseState[i] = mouseNormal;
        }
        state = -1;
    }

    void paint (juce::Graphics& g) override{
        
        // draw button
        g.setColour(juce::Colours::black);
        for (int i = 0; i < 5; i++) {
            if (mouseState[i] == mouseNormal) g.setOpacity(1.0);
            else if (mouseState[i] == mouseOver) g.setOpacity(0.8);
            else if (mouseState[i] == mouseDown) g.setOpacity(0.6);

            g.drawImageAt(cachedImage[i], buttonSize * i, 0);
        }
    }
    
    int getState() { return state; }
    
private:
    void timerCallback() override
    {
        repaint();
    }
    
    enum MouseState { mouseNormal, mouseOver, mouseDown };

    int buttonSize = 58;
    juce::Image cachedImage[5];
    MouseState mouseState[5];
    int state = -1;
    
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
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MomentaryRadioButton)

};
