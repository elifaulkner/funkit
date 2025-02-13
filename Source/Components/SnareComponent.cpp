/*
  ==============================================================================

    SnareComponent.cpp
    Created: 7 Feb 2025 1:09:15pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SnareComponent.h"

//==============================================================================
SnareComponent::SnareComponent(FunkitAudioProcessor& ap, juce::AudioProcessorValueTreeState& apvts) :
    _processor(ap),
    _noteSlider("Tune", apvts, "SNARE_NOTE"),
    _levelSlider("Level", apvts, "SNARE_LEVEL"),
    _decaySlider("Decay", apvts, "SNARE_DECAY"),
    _shapeSlider("Shape", apvts, "SNARE_SHAPE"),
    _driveSlider("Drive", apvts, "SNARE_DRIVE"),
    _noiseSlider("Noise", apvts, "SNARE_NOISE"),
    _reverbSlider("Verb", apvts, "SNARE_REVERB"),
    _reverbSizeSlider("Verb Size", apvts, "SNARE_REVERB_SIZE"),
    _gateSlider("Gate", apvts, "SNARE_GATE_THRESHOLD"),
    _trigger("Trigger (D2)")
{
    addAndMakeVisible(_noteSlider);
    addAndMakeVisible(_levelSlider);
    addAndMakeVisible(_decaySlider);
    addAndMakeVisible(_shapeSlider);
    addAndMakeVisible(_driveSlider);
    addAndMakeVisible(_noiseSlider);
    addAndMakeVisible(_reverbSlider);
    addAndMakeVisible(_reverbSizeSlider);
    addAndMakeVisible(_gateSlider);
    
    addAndMakeVisible(_label);
    
    addAndMakeVisible(_trigger);
    _trigger.addListener(this);
}

SnareComponent::~SnareComponent()
{
}

void SnareComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::grey);
}

void SnareComponent::resized()
{
    _label.setBounds(getBounds().getCentreX()-50, 0, 100, 25);
    _noteSlider.setBounds(0, 25, 75, 75);
    _levelSlider.setBounds(75, 25, 75, 75);
    _decaySlider.setBounds(150, 25, 75, 75);
    _driveSlider.setBounds(225, 25, 75, 75);
    _noiseSlider.setBounds(300, 25, 75, 75);
    _shapeSlider.setBounds(375, 25, 75, 75);
    _reverbSlider.setBounds(450, 25, 75, 75);
    _reverbSizeSlider.setBounds(525, 25, 75, 75);
    _gateSlider.setBounds(600, 25, 75, 75);
    
    _trigger.setBounds(700, 40, _trigger.getBestWidthForHeight(40), 40);
}


void SnareComponent::buttonClicked (juce::Button *button) {
}

void SnareComponent::buttonStateChanged (juce::Button* button) {
    if(button->isDown()) {
        _processor.triggerSnare(1.0f);
    }
}
