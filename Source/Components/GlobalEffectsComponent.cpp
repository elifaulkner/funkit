/*
  ==============================================================================

    GlobalEffectsComponent.cpp
    Created: 13 Feb 2025 10:39:12am
    Author:  Eli Faulkner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GlobalEffectsComponent.h"

//==============================================================================
GlobalEffectsComponent::GlobalEffectsComponent(FunkitAudioProcessor& ap, juce::AudioProcessorValueTreeState& apvts) : _processor(ap),
    _delaySlider("Delay", apvts, "GLOBAL_DELAY"),
    _delayLevelSlider("Delay Level", apvts, "GLOBAL_DELAY_LEVEL"),
    _delayFeedbackSlider("Delay Feedback", apvts, "GLOBAL_DELAY_FEEDBACK"),
    _cutoffSlider("Cutoff", apvts, "GLOBAL_CUTOFF"),
    _resonanceSlider("Resonance", apvts, "GLOBAL_RES"),
_saturationSlider("Saturation", apvts, "GLOBAL_SATURATION_LEVEL")
{
    addAndMakeVisible(_delaySlider);
    addAndMakeVisible(_delayLevelSlider);
    addAndMakeVisible(_delayFeedbackSlider);
    addAndMakeVisible(_cutoffSlider);
    addAndMakeVisible(_resonanceSlider);
    addAndMakeVisible(_saturationSlider);
    
    FunkitLookAndFeel::styleTitle(_label);
    
    addAndMakeVisible(_label);
}

GlobalEffectsComponent::~GlobalEffectsComponent()
{
}

void GlobalEffectsComponent::paint (juce::Graphics& g)
{
    FunkitLookAndFeel::paintPanel(g, getLocalBounds());
}

void GlobalEffectsComponent::resized()
{
    FunkitLookAndFeel::layoutSection(getLocalBounds(), _label, nullptr,
        { &_delaySlider, &_delayLevelSlider, &_delayFeedbackSlider,
          &_cutoffSlider, &_resonanceSlider, &_saturationSlider });
}
