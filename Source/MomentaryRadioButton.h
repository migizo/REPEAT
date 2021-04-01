/*
  ==============================================================================

    MomentaryRadioButton.h
    Created: 31 Mar 2021 3:14:27pm
    Author:  migizo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "AttachedControlBase.h"
#define BUTTON_NUM 6
using namespace juce;

//==============================================================================
class MomentaryRadioButton : public Component, private juce::Timer{
public:
    MomentaryRadioButton();
    virtual ~MomentaryRadioButton(){
        stopTimer();
    }
    
    void timerCallback() override {repaint();}
    
    class Listener {
    public:
        virtual void stateChanged(MomentaryRadioButton* btn, int state) = 0;
    };
    
    void paint(Graphics& g) override;
    // void resized() override{};
    
    void mouseMove(const juce::MouseEvent& event) override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override ;
    void mouseExit(const juce::MouseEvent& event) override;
    void onMouseSelect(const juce::MouseEvent& event);
    
    void setState(int newState, const NotificationType notification);
    void setState();
    int getState() { return state;}
    
    void addListener(Listener* listener) {listeners.add(listener); }
    void removeListener(Listener* listener) {listeners.remove(listener); }
    void setDefaultState(float newDefaultState) {defaultState = newDefaultState; }

private:
	ListenerList<Listener> listeners;
 	
    int itemNum = BUTTON_NUM;
    float itemSize = 48.3333f;
    int defaultState, state;
    juce::Image cachedImage[BUTTON_NUM];
    int hoverID = 0;
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

//==============================================================================
class MomentaryRadioButtonAttachment : public AttachedControlBase, public MomentaryRadioButton::Listener {
public:

	MomentaryRadioButtonAttachment(AudioProcessorValueTreeState& s, const String& p, MomentaryRadioButton& mrb, bool populateGroup = true);
	~MomentaryRadioButtonAttachment();
    void setValue(float newValue) override;

    void stateChanged(MomentaryRadioButton* btn, int newState) override;
    
private:
	MomentaryRadioButton& mrb;
	ScopedPointer<NormalisableRange<float>>range;
	float invInterval;
	
	int valueToState(float value);
	float stateToValue(int state);

	bool ignoreCallbacks;
	CriticalSection selfCallbackMutex;
	

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MomentaryRadioButtonAttachment)
};
