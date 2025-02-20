/*
  ==============================================================================

    HiHatComponent.h
    Created: 12 Feb 2025 11:20:56am
    Author:  Eli Faulkner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LabeledSlider.h"
#include "../PluginProcessor.h"
#include "Util/LabeledToggle.h"

//==============================================================================
/*
*/
class HiHatComponent  : public juce::Component, public juce::Button::Listener
{
public:
    HiHatComponent(FunkitAudioProcessor& ap, juce::AudioProcessorValueTreeState& apvts);
    ~HiHatComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked (juce::Button *button) override;
    void buttonStateChanged (juce::Button* button) override;
    
private:
    FunkitAudioProcessor& _processor;
    juce::Label _label {"HiHat", "HiHat"};
    LabeledSlider _levelSlider;
    LabeledSlider _decaySlider;
    LabeledSlider _driveSlider;
    LabeledSlider _noiseSlider;
    LabeledSlider _shapeSlider;

    LabeledToggle _globalToggle;
    
    juce::TextButton _trigger;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HiHatComponent)
};
