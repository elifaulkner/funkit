/*
  ==============================================================================

    LabeledSlider.cpp
    Created: 7 Feb 2025 12:03:05pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LabeledSlider.h"
#include "Util/FunkitLookAndFeel.h"

//==============================================================================
LabeledSlider::LabeledSlider(juce::String label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID) : _label(label, label)
{
    _slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    _slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    _slider.setPopupDisplayEnabled(true, true, nullptr);
    addAndMakeVisible(_slider);
    
    _label.setColour(juce::Label::ColourIds::textColourId, FunkitColours::textDim);
    _label.setJustificationType(juce::Justification::centred);
    _label.setFont(juce::FontOptions(12.5f));
    _label.setMinimumHorizontalScale(0.7f);
    addAndMakeVisible(_label);
    
    _attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, _slider);
}

LabeledSlider::~LabeledSlider()
{
}

void LabeledSlider::paint (juce::Graphics& g)
{
    // Transparent: the parent panel provides the background.
}

void LabeledSlider::resized()
{
    auto area = getLocalBounds();
    _label.setBounds(area.removeFromBottom(22));
    _slider.setBounds(area);
}

void LabeledSlider::showAsNoteName()
{
    _slider.textFromValueFunction = [](double value) {
        return juce::MidiMessage::getMidiNoteName((int) std::round(value), true, true, 3);
    };
    _slider.valueFromTextFunction = [](const juce::String& text) {
        for(int n = 0; n < 128; ++n) {
            if(juce::MidiMessage::getMidiNoteName(n, true, true, 3).equalsIgnoreCase(text.trim())) {
                return (double) n;
            }
        }
        return text.getDoubleValue();
    };
    // Force the popup/text to refresh with the new formatting.
    _slider.updateText();
}
