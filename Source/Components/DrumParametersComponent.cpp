/*
  ==============================================================================

    DrumParametersComponent.cpp
    Created: 20 Feb 2025 1:49:32pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DrumParametersComponent.h"

//==============================================================================
DrumParametersComponent::DrumParametersComponent(FunkitAudioProcessor& p) : _audioProcessor (p), _kick(p, p.getApvts()), _snare(p, p.getApvts()), _hiHat(p, p.getApvts()), _global(p, p.getApvts())
{
    setSize (850, 400);
    addAndMakeVisible(_kick);
    addAndMakeVisible(_snare);
    addAndMakeVisible(_hiHat);
    addAndMakeVisible(_global);
}

DrumParametersComponent::~DrumParametersComponent()
{
}

void DrumParametersComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::grey);
}

void DrumParametersComponent::resized()
{
    _kick.setBounds(0, 0, 850, 100);
    _snare.setBounds(0, 100, 850, 100);
    _hiHat.setBounds(0, 200, 850, 100);
    _global.setBounds(0, 300, 850, 100);
}
