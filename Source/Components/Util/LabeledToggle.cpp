/*
  ==============================================================================

    LabeledToggle.cpp
    Created: 20 Feb 2025 4:06:07pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LabeledToggle.h"

//==============================================================================
LabeledToggle::LabeledToggle(juce::String label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID) : _label(label, label)
{
    addAndMakeVisible(_button);
    
    _label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    _label.setJustificationType(juce::Justification::centred);
    _label.setFont(15);
    
    _attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, paramID, _button);
    
    addAndMakeVisible(_label);
}

LabeledToggle::~LabeledToggle()
{
}

void LabeledToggle::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::grey);
}

void LabeledToggle::resized()
{
    _button.setBounds(25, 0, 75, 50);
    _label.setBounds(0, 50, 75, 25);
}
