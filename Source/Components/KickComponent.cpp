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
    _shapeSlider("Shape", apvts, "KICK_SHAPE"),
    _trigger("Trigger (C2)")
{
    addAndMakeVisible(_noteSlider);
    addAndMakeVisible(_levelSlider);
    addAndMakeVisible(_decaySlider);
    addAndMakeVisible(_driveSlider);
    addAndMakeVisible(_noiseSlider);
    addAndMakeVisible(_shapeSlider);
    addAndMakeVisible(_trigger);
    
    _label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    _label.setJustificationType(juce::Justification::centred);
    _label.setFont(22);
    
    _trigger.addListener(this);
    
    addAndMakeVisible(_label);
}

KickComponent::~KickComponent()
{
    _trigger.removeListener(this);
}

void KickComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::grey);
}

void KickComponent::resized()
{
    auto area = getLocalBounds();
    
    int labelHeight = 25;
    
    _label.setBounds(area.removeFromTop(labelHeight));
    
    int sliderWidth = 75;
    
    _noteSlider.setBounds(area.removeFromLeft(sliderWidth));
    _levelSlider.setBounds(area.removeFromLeft(sliderWidth));
    _decaySlider.setBounds(area.removeFromLeft(sliderWidth));
    _driveSlider.setBounds(area.removeFromLeft(sliderWidth));
    _noiseSlider.setBounds(area.removeFromLeft(sliderWidth));
    _shapeSlider.setBounds(area.removeFromLeft(sliderWidth));
    
    _trigger.setBounds(700, 40, _trigger.getBestWidthForHeight(40), 40);
}

void KickComponent::buttonClicked (juce::Button *button) {
   // _processor.triggerKick(1.0f);
}

void KickComponent::buttonStateChanged (juce::Button* button) {
    if(button->isDown()) {
        _processor.triggerKick(1.0f);
    }
}

