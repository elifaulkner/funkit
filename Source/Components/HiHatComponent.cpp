/*
  ==============================================================================

    HiHatComponent.cpp
    Created: 12 Feb 2025 11:20:56am
    Author:  Eli Faulkner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "HiHatComponent.h"

//==============================================================================
HiHatComponent::HiHatComponent(FunkitAudioProcessor& ap, juce::AudioProcessorValueTreeState& apvts) :
    _processor(ap),
    _levelSlider("Level", apvts, "HIHAT_LEVEL"),
    _decaySlider("Decay", apvts, "HIHAT_DECAY"),
    _driveSlider("Drive", apvts, "HIHAT_DRIVE"),
    _noiseSlider("Noise", apvts, "HIHAT_NOISE"),
    _shapeSlider("Shape", apvts, "HIHAT_SHAPE"),
    _delaySlider("Delay", apvts, "HIHAT_DELAY"),
    _delayLevelSlider("Delay Level", apvts, "HIHAT_DELAY_LEVEL"),
    _delayFeedbackSlider("Delay Feedback", apvts, "HIHAT_DELAY_FEEDBACK"),
    _trigger("Trigger (F#2)")
{
    addAndMakeVisible(_levelSlider);
    addAndMakeVisible(_decaySlider);
    addAndMakeVisible(_driveSlider);
    addAndMakeVisible(_noiseSlider);
    addAndMakeVisible(_shapeSlider);
    addAndMakeVisible(_trigger);
    addAndMakeVisible(_delaySlider);
    addAndMakeVisible(_delayLevelSlider);
    addAndMakeVisible(_delayFeedbackSlider);
    
    FunkitLookAndFeel::styleTitle(_label);
    
    _trigger.addListener(this);
    
    addAndMakeVisible(_label);
}

HiHatComponent::~HiHatComponent()
{
}

void HiHatComponent::paint (juce::Graphics& g)
{
    FunkitLookAndFeel::paintPanel(g, getLocalBounds());
}

void HiHatComponent::resized()
{
    FunkitLookAndFeel::layoutSection(getLocalBounds(), _label, &_trigger,
        { &_levelSlider, &_decaySlider, &_shapeSlider, &_noiseSlider,
          &_driveSlider, &_delaySlider, &_delayLevelSlider, &_delayFeedbackSlider });
}

void HiHatComponent::buttonClicked (juce::Button *button) {
}

void HiHatComponent::buttonStateChanged (juce::Button* button) {
    if(button->isDown()) {
        _processor.triggerHiHat(1.0f);
    }
}
