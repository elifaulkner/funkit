/*
  ==============================================================================

    LabeledSlider.cpp
    Created: 7 Feb 2025 12:03:05pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LabeledSlider.h"

//==============================================================================
LabeledSlider::LabeledSlider(juce::String label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID) : _label(label, label)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    _slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    _slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    addAndMakeVisible(_slider);
    
    _label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    _label.setJustificationType(juce::Justification::centred);
    _label.setFont(15);
    
    _attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, _slider);
    addAndMakeVisible(_label);
}

LabeledSlider::~LabeledSlider()
{
}

void LabeledSlider::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (juce::Colours::grey);   // clear the background
}

void LabeledSlider::resized()
{
    _slider.setBounds(0, 0, 75, 50);
    _label.setBounds(0, 50, 75, 25);
}
