/*
  ==============================================================================

    WoodComponent.h
    Created: 17 Mar 2025 4:51:39pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "LabeledSlider.h"

//==============================================================================
/*
*/
class WoodComponent  : public juce::Component, public juce::Button::Listener
{
public:
    WoodComponent(FunkitAudioProcessor& ap, juce::AudioProcessorValueTreeState& apvts);
    ~WoodComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void buttonClicked (juce::Button *button) override;
    void buttonStateChanged (juce::Button* button) override;
private:
    FunkitAudioProcessor& _processor;
    juce::Label _label {"Wood", "Wood Block"};
    LabeledSlider _noteSlider;
    LabeledSlider _levelSlider;
    LabeledSlider _decaySlider;
    
    juce::TextButton _trigger;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WoodComponent)
};
