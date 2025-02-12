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
    _trigger("Trigger")
{
    addAndMakeVisible(_levelSlider);
    addAndMakeVisible(_decaySlider);
    addAndMakeVisible(_driveSlider);
    addAndMakeVisible(_noiseSlider);
    addAndMakeVisible(_trigger);
    
    _label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    _label.setJustificationType(juce::Justification::centred);
    _label.setFont(15);
    
    _trigger.addListener(this);
    
    addAndMakeVisible(_label);
}

HiHatComponent::~HiHatComponent()
{
}

void HiHatComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::grey);
}

void HiHatComponent::resized()
{
    _label.setBounds(getBounds().getCentreX()-50, 0, 100, 25);
    _levelSlider.setBounds(75, 25, 75, 75);
    _decaySlider.setBounds(150, 25, 75, 75);
    _driveSlider.setBounds(225, 25, 75, 75);
    _noiseSlider.setBounds(300, 25, 75, 75);
    
    _trigger.setBounds(600, 40, _trigger.getBestWidthForHeight(40), 40);
}

void HiHatComponent::buttonClicked (juce::Button *button) {
}

void HiHatComponent::buttonStateChanged (juce::Button* button) {
    if(button->isDown()) {
        _processor.triggerHiHat(1.0f);
    }
}
