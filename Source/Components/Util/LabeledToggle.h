/*
  ==============================================================================

    LabeledToggle.h
    Created: 20 Feb 2025 4:06:07pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LabeledToggle  : public juce::Component
{
public:
    LabeledToggle(juce::String label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID);
    ~LabeledToggle() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ToggleButton _button;
    juce::Label _label;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> _attachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LabeledToggle)
};
