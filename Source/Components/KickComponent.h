/*
  ==============================================================================

    KickComponent.h
    Created: 7 Feb 2025 11:18:21am
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
class KickComponent  : public juce::Component, public juce::Button::Listener
{
public:
    KickComponent(FunkitAudioProcessor& ap, juce::AudioProcessorValueTreeState& apvts);
    ~KickComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked (juce::Button *button) override;
    void buttonStateChanged (juce::Button* button) override;
private:
    FunkitAudioProcessor& _processor;
    juce::Label _label {"Kick", "Kick Drum"};
    LabeledSlider _noteSlider;
    LabeledSlider _levelSlider;
    LabeledSlider _decaySlider;
    LabeledSlider _driveSlider;
    LabeledSlider _noiseSlider;
    
    juce::TextButton _trigger;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KickComponent)
};
