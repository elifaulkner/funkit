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
    _impactSlider("Impact", apvts, "SNARE_IMPACT"),
    _fmSlider("FM", apvts, "SNARE_FM"),
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
    addAndMakeVisible(_impactSlider);
    addAndMakeVisible(_fmSlider);
    
    _label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    _label.setJustificationType(juce::Justification::centred);
    _label.setFont(22);
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
    auto area = getLocalBounds();
    
    int labelHeight = 25;
    
    _label.setBounds(area.removeFromTop(labelHeight));
    
    int sliderWidth = 75;
    
    _noteSlider.setBounds(area.removeFromLeft(sliderWidth));
    _levelSlider.setBounds(area.removeFromLeft(sliderWidth));
    _decaySlider.setBounds(area.removeFromLeft(sliderWidth));
    _shapeSlider.setBounds(area.removeFromLeft(sliderWidth));
    _driveSlider.setBounds(area.removeFromLeft(sliderWidth));
    _impactSlider.setBounds(area.removeFromLeft(sliderWidth));
    _noiseSlider.setBounds(area.removeFromLeft(sliderWidth));
    _fmSlider.setBounds(area.removeFromLeft(sliderWidth));
    _reverbSlider.setBounds(area.removeFromLeft(sliderWidth));
    _reverbSizeSlider.setBounds(area.removeFromLeft(sliderWidth));
    _gateSlider.setBounds(area.removeFromLeft(sliderWidth));
    
    _trigger.setBounds(850, 40, _trigger.getBestWidthForHeight(40), 40);
}


void SnareComponent::buttonClicked (juce::Button *button) {
}

void SnareComponent::buttonStateChanged (juce::Button* button) {
    if(button->isDown()) {
        _processor.triggerSnare(1.0f);
    }
}
