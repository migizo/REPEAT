/*
  ==============================================================================

    MomentaryRadioButton.cpp
    Created: 31 Mar 2021 3:14:27pm
    Author:  migizo

  ==============================================================================
*/

#include "MomentaryRadioButton.h"
#include "ButtonResource.h"

const char* MomentaryRadioButton::largeButton1_png = (const char*) resource_largeButton1_png;
const int MomentaryRadioButton::largeButton1_pngSize = 910;
const char* MomentaryRadioButton::largeButton2_png = (const char*) resource_largeButton2_png;
const int MomentaryRadioButton::largeButton2_pngSize = 1087;
const char* MomentaryRadioButton::largeButton4_png = (const char*) resource_largeButton4_png;
const int MomentaryRadioButton::largeButton4_pngSize = 1011;
const char* MomentaryRadioButton::largeButton8_png = (const char*) resource_largeButton8_png;
const int MomentaryRadioButton::largeButton8_pngSize = 1152;
const char* MomentaryRadioButton::largeButton16_png = (const char*) resource_largeButton16_png;
const int MomentaryRadioButton::largeButton16_pngSize = 1179;
const char* MomentaryRadioButton::largeButton32_png = (const char*) resource_largeButton32_png;
const int MomentaryRadioButton::largeButton32_pngSize = 1326;

//==============================================================================
MomentaryRadioButton::MomentaryRadioButton() {
    cachedImage[0] = juce::ImageCache::getFromMemory (largeButton1_png, largeButton1_pngSize);
    cachedImage[1] = juce::ImageCache::getFromMemory (largeButton2_png, largeButton2_pngSize);
    cachedImage[2] = juce::ImageCache::getFromMemory (largeButton4_png, largeButton4_pngSize);
    cachedImage[3] = juce::ImageCache::getFromMemory (largeButton8_png, largeButton8_pngSize);
    cachedImage[4] = juce::ImageCache::getFromMemory (largeButton16_png, largeButton16_pngSize);
    cachedImage[5] = juce::ImageCache::getFromMemory (largeButton32_png, largeButton32_pngSize);
    startTimerHz(60);
}

//------------------------------------------------------------------------
void MomentaryRadioButton::paint(Graphics& g) {
    // draw button
    g.setColour(juce::Colours::black);
    for (int i = 0; i < itemNum; i++) {
        if (state - 1 == i) g.setOpacity(0.8);
        else if (hoverID - 1 == i) g.setOpacity(0.9);
        else g.setOpacity(1.0);
        g.drawImageAt(cachedImage[i], itemSize * i, 0);
    }
}
    
//------------------------------------------------------------------------
void MomentaryRadioButton::mouseMove(const juce::MouseEvent& event) {
    float mouseX = event.position.x;
    hoverID = 0;
    for (int i = 0; i < itemNum; i++) {
        if (itemSize * i <= mouseX && mouseX < itemSize * (i + 1)) {
            hoverID = i + 1;
            break;
        }
    }
}
    
//------------------------------------------------------------------------
void MomentaryRadioButton::mouseDrag(const juce::MouseEvent& event) {
	
    int mouseX = event.position.x;
    isClick = true;
    int _state = 0;
    for (int i = 0; i < itemNum; i++) {
        if (itemSize * i <= mouseX && mouseX < itemSize * (i + 1)) {
            _state = i + 1;
            hoverID = 0;
        }
    }
    if (state != _state) {
        state = _state;
       	listeners.call(&Listener::stateChanged, this, state);
    }
}
    
//------------------------------------------------------------------------
void MomentaryRadioButton::mouseUp(const juce::MouseEvent& event) {
    if (isClick) {
        isClick = false;
        listeners.call(&Listener::stateChanged, this, 0);
    }
        
    float mouseX = event.position.x;
    hoverID = 0;
    for (int i = 0; i < itemNum; i++) {
        if (itemSize * i <= mouseX && mouseX < itemSize * (i + 1)) {
            hoverID = i + 1;
            break;
        }
    }
}
    
//------------------------------------------------------------------------
void MomentaryRadioButton::mouseExit(const juce::MouseEvent& event) {
    if (isClick) {
        isClick = false;
	    listeners.call(&Listener::stateChanged, this, 0);
    }
    hoverID = 0;
}
    
//------------------------------------------------------------------------
void MomentaryRadioButton::setState(int newState, const NotificationType notification) {
    if (newState >= 0)
    state = (newState >= 0 && newState <= itemNum) ? newState : defaultState;
    if (notification != dontSendNotification)
        listeners.call(&Listener::stateChanged, this, state);
}

//------------------------------------------------------------------------
void MomentaryRadioButton::setState() {
    state = defaultState;
}

//==============================================================================


MomentaryRadioButtonAttachment::MomentaryRadioButtonAttachment(AudioProcessorValueTreeState& s, const String& p, MomentaryRadioButton& mrb, bool populateGroup)
    : AttachedControlBase(s, p), mrb(mrb),  ignoreCallbacks(false)
{
    range = new NormalisableRange<float>(s.getParameterRange(p));
    if (range->interval == 0.f) return;
	invInterval = 1.f / range->interval;
		
    mrb.setDefaultState(valueToState(getParameter()->get())); // mod
    mrb.resized();
    sendInitialUpdate();
    mrb.addListener(this);
}

//------------------------------------------------------------------------
MomentaryRadioButtonAttachment::~MomentaryRadioButtonAttachment() {
    AttachedControlBase::removeListener();
    mrb.removeListener(this);
}

//------------------------------------------------------------------------
void MomentaryRadioButtonAttachment::setValue(float newValue) {
    const ScopedLock selfCallbackLock(selfCallbackMutex);
    {
        ScopedValueSetter<bool> svs(ignoreCallbacks, true);
        mrb.setState(valueToState(newValue), sendNotificationSync);
    }
}

//------------------------------------------------------------------------
void MomentaryRadioButtonAttachment::stateChanged(MomentaryRadioButton* btn, int newState) {
    const ScopedLock selfCallbackLock(selfCallbackMutex);
	if ((!ignoreCallbacks) && (!ModifierKeys::getCurrentModifiers().isRightButtonDown()))
		setNewUnnormalisedValue(stateToValue(newState));
}

//------------------------------------------------------------------------
int MomentaryRadioButtonAttachment::valueToState(float value) {
    return roundToInt(invInterval * (value - range->start));
}

//------------------------------------------------------------------------
float MomentaryRadioButtonAttachment::stateToValue(int state) {
    return range->start + (float)state * range->interval;
}
