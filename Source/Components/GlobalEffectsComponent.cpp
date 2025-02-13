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
    _cutoffSlider("Cutoff", apvts, "GLOBAL_CUTOFF"),
    _resonanceSlider("Resonance", apvts, "GLOBAL_RES")
{
    addAndMakeVisible(_delaySlider);
    addAndMakeVisible(_delayLevelSlider);
    addAndMakeVisible(_cutoffSlider);
    addAndMakeVisible(_resonanceSlider);
    
    _label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    _label.setJustificationType(juce::Justification::centred);
    _label.setFont(15);
    
    addAndMakeVisible(_label);
}

GlobalEffectsComponent::~GlobalEffectsComponent()
{
}

void GlobalEffectsComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::grey);  
}

void GlobalEffectsComponent::resized()
{
    _label.setBounds(getBounds().getCentreX()-50, 0, 100, 25);
    
    _delaySlider.setBounds(0, 25, 75, 75);
    _delayLevelSlider.setBounds(75, 25, 75, 75);
    _cutoffSlider.setBounds(150, 25, 75, 75);
    _resonanceSlider.setBounds(225, 25, 75, 75);
}
