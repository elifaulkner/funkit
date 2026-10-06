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
    
    FunkitLookAndFeel::styleTitle(_label);
    addAndMakeVisible(_label);
    
    addAndMakeVisible(_trigger);
    _trigger.addListener(this);
}

SnareComponent::~SnareComponent()
{
}

void SnareComponent::paint (juce::Graphics& g)
{
    FunkitLookAndFeel::paintPanel(g, getLocalBounds());
}

void SnareComponent::resized()
{
    FunkitLookAndFeel::layoutSection(getLocalBounds(), _label, &_trigger,
        { &_noteSlider, &_levelSlider, &_decaySlider, &_shapeSlider, &_driveSlider, &_impactSlider,
          &_noiseSlider, &_fmSlider, &_reverbSlider, &_reverbSizeSlider, &_gateSlider });
}


void SnareComponent::buttonClicked (juce::Button *button) {
}

void SnareComponent::buttonStateChanged (juce::Button* button) {
    if(button->isDown()) {
        _processor.triggerSnare(1.0f);
    }
}
