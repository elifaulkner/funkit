/*
  ==============================================================================

    KickComponent.cpp
    Created: 7 Feb 2025 11:18:21am
    Author:  Eli Faulkner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "KickComponent.h"

//==============================================================================
KickComponent::KickComponent(FunkitAudioProcessor& ap, juce::AudioProcessorValueTreeState& apvts) :
    _processor(ap),
    _noteSlider("Tune", apvts, "KICK_NOTE"),
    _levelSlider("Level", apvts, "KICK_LEVEL"),
    _decaySlider("Decay", apvts, "KICK_DECAY"),
    _driveSlider("Drive", apvts, "KICK_DRIVE"),
    _noiseSlider("Noise", apvts, "KICK_NOISE"),
    _trigger("Trigger")
{
    addAndMakeVisible(_noteSlider);
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

KickComponent::~KickComponent()
{
    _trigger.removeListener(this);
}

void KickComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::grey);   // clear the background
}

void KickComponent::resized()
{
    _label.setBounds(getBounds().getCentreX()-50, 0, 100, 25);
    _noteSlider.setBounds(0, 25, 75, 75);
    _levelSlider.setBounds(75, 25, 75, 75);
    _decaySlider.setBounds(150, 25, 75, 75);
    _driveSlider.setBounds(225, 25, 75, 75);
    _noiseSlider.setBounds(300, 25, 75, 75);
    
    _trigger.setBounds(600, 40, _trigger.getBestWidthForHeight(40), 40);
}

void KickComponent::buttonClicked (juce::Button *button) {
   // _processor.triggerKick(1.0f);
}

void KickComponent::buttonStateChanged (juce::Button* button) {
    if(button->isDown()) {
        _processor.triggerKick(1.0f);
    }
}

