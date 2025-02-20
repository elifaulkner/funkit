/*
  ==============================================================================

    GlobalEffectsComponent.h
    Created: 13 Feb 2025 10:39:12am
    Author:  Eli Faulkner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LabeledSlider.h"
#include "../PluginProcessor.h"

//==============================================================================
/*
*/
class GlobalEffectsComponent  : public juce::Component
{
public:
    GlobalEffectsComponent(FunkitAudioProcessor& ap, juce::AudioProcessorValueTreeState& apvts);
    ~GlobalEffectsComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    FunkitAudioProcessor& _processor;
    juce::Label _label {"Global Effects", "Global Effects"};
    
    LabeledSlider _delaySlider;
    LabeledSlider _delayLevelSlider;
    LabeledSlider _delayFeedbackSlider;
    LabeledSlider _cutoffSlider;
    LabeledSlider _resonanceSlider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlobalEffectsComponent)
};
