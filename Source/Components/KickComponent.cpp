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
    _fmSlider("FM", apvts, "KICK_FM"),
    _impactSlider("Impact", apvts, "KICK_IMPACT"),
    _trigger("Trigger (C2)")
{
    addAndMakeVisible(_noteSlider);
    addAndMakeVisible(_levelSlider);
    addAndMakeVisible(_decaySlider);
    addAndMakeVisible(_driveSlider);
    addAndMakeVisible(_noiseSlider);
    addAndMakeVisible(_shapeSlider);
    addAndMakeVisible(_trigger);
    addAndMakeVisible(_impactSlider);
    addAndMakeVisible(_fmSlider);
    
    FunkitLookAndFeel::styleTitle(_label);
    
    _trigger.addListener(this);
    
    addAndMakeVisible(_label);
}

KickComponent::~KickComponent()
{
    _trigger.removeListener(this);
}

void KickComponent::paint (juce::Graphics& g)
{
    FunkitLookAndFeel::paintPanel(g, getLocalBounds());
}

void KickComponent::resized()
{
    FunkitLookAndFeel::layoutSection(getLocalBounds(), _label, &_trigger,
        { &_noteSlider, &_levelSlider, &_decaySlider, &_shapeSlider,
          &_impactSlider, &_noiseSlider, &_fmSlider, &_driveSlider });
}

void KickComponent::buttonClicked (juce::Button *button) {
   // _processor.triggerKick(1.0f);
}

void KickComponent::buttonStateChanged (juce::Button* button) {
    if(button->isDown()) {
        _processor.triggerKick(1.0f);
    }
}

