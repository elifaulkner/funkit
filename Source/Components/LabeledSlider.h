/*
  ==============================================================================

    LabeledSlider.h
    Created: 7 Feb 2025 12:03:05pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LabeledSlider  : public juce::Component
{
public:
    LabeledSlider(juce::String label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID);
    ~LabeledSlider() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider _slider;
    juce::Label _label;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _attachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LabeledSlider)
};
