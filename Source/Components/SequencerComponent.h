/*
  ==============================================================================

    SequencerComponent.h
    Created: 20 Feb 2025 1:48:24pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class SequencerComponent  : public juce::Component
{
public:
    SequencerComponent();
    ~SequencerComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SequencerComponent)
};
