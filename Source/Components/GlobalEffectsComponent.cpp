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
    
    _label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    _label.setJustificationType(juce::Justification::centred);
    _label.setFont(22);
    
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
    auto area = getLocalBounds();
    
    int labelHeight = 25;
    
    _label.setBounds(area.removeFromTop(labelHeight));
    
    int sliderWidth = 75;
    
    _delaySlider.setBounds(area.removeFromLeft(sliderWidth));
    _delayLevelSlider.setBounds(area.removeFromLeft(sliderWidth));
    _delayFeedbackSlider.setBounds(area.removeFromLeft(sliderWidth));
    _cutoffSlider.setBounds(area.removeFromLeft(sliderWidth));
    _resonanceSlider.setBounds(area.removeFromLeft(sliderWidth));
    _saturationSlider.setBounds(area.removeFromLeft(sliderWidth));
}
