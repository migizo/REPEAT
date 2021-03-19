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
#define BTN_NUM 6
class MomentaryRadioButton : public juce::Component, private juce::Timer{
  
public:
    
    MomentaryRadioButton() {
        cachedImage[0] = juce::ImageCache::getFromMemory (largeButton1_png, largeButton1_pngSize);
        cachedImage[1] = juce::ImageCache::getFromMemory (largeButton2_png, largeButton2_pngSize);
        cachedImage[2] = juce::ImageCache::getFromMemory (largeButton4_png, largeButton4_pngSize);
        cachedImage[3] = juce::ImageCache::getFromMemory (largeButton8_png, largeButton8_pngSize);
        cachedImage[4] = juce::ImageCache::getFromMemory (largeButton16_png, largeButton16_pngSize);
        cachedImage[5] = juce::ImageCache::getFromMemory (largeButton32_png, largeButton32_pngSize);
        startTimerHz(60);
    }
    ~MomentaryRadioButton() {
        stopTimer();
    }
    
    // check hover
    void mouseMove(const juce::MouseEvent& event) override {
        float mouseX = event.position.x;
        hoverButtonID = 0;
        for (int i = 0; i < BTN_NUM; i++) {
            if (buttonSize * i <= mouseX && mouseX < buttonSize * (i + 1)) {
                hoverButtonID = i + 1;
                break;
            }
        }
    }
    
    void mouseDrag(const juce::MouseEvent& event) override {
        float mouseX = event.position.x;
        isClick = true;
        int state = 0;
        for (int i = 0; i < BTN_NUM; i++) {
            if (buttonSize * i <= mouseX && mouseX < buttonSize * (i + 1)) {
                state = i + 1;
                hoverButtonID = 0;
            }
        }
        setValue(state);
    }
    void mouseUp(const juce::MouseEvent& event) override {
        if (isClick) {
            isClick = false;
            setValue(0);
        }
        
        float mouseX = event.position.x;
        hoverButtonID = 0;
        for (int i = 0; i < BTN_NUM; i++) {
            if (buttonSize * i <= mouseX && mouseX < buttonSize * (i + 1)) {
                hoverButtonID = i + 1;
                break;
            }
        }
    }
    
    // set default
    void mouseExit(const juce::MouseEvent& event) override {
        if (isClick) {
            isClick = false;
            setValue(0);
        }
        hoverButtonID = 0;
    }

    void paint (juce::Graphics& g) override{
        
        // draw button
        g.setColour(juce::Colours::black);
        for (int i = 0; i < BTN_NUM; i++) {
            if (value - 1 == i) g.setOpacity(0.6);
            else if (hoverButtonID - 1 == i) g.setOpacity(0.8);
            else g.setOpacity(1.0);
            g.drawImageAt(cachedImage[i], buttonSize * i, 0);
        }
    }
    
    int getValue() { return value; }
    void setValue(float newValue) {
        value = newValue;
    }

    bool checkChange() {
        bool lastIsChange = isChange;
        if (isChange)isChange = false;
        return lastIsChange;
    }
private:
    void timerCallback() override
    {
        static int lastValue = 0;
        if (lastValue != value) isChange = true;
        lastValue = value;
        
        repaint();
    }
    
    float buttonSize = 48.3333;
    juce::Image cachedImage[BTN_NUM];
    int hoverButtonID = 0;
    int value = 0;
    bool isChange = false;
    bool isClick = false;
    
    static const char* largeButton1_png;
    static const int largeButton1_pngSize;
    static const char* largeButton2_png;
    static const int largeButton2_pngSize;
    static const char* largeButton4_png;
    static const int largeButton4_pngSize;
    static const char* largeButton8_png;
    static const int largeButton8_pngSize;
    static const char* largeButton16_png;
    static const int largeButton16_pngSize;
    static const char* largeButton32_png;
    static const int largeButton32_pngSize;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MomentaryRadioButton)

};
