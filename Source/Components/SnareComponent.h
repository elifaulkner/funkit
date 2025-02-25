/*
  ==============================================================================

    SnareComponent.h
    Created: 7 Feb 2025 1:09:15pm
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
class SnareComponent  : public juce::Component, public juce::Button::Listener
{
public:
    SnareComponent(FunkitAudioProcessor& ap, juce::AudioProcessorValueTreeState& apvts);
    ~SnareComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked (juce::Button *button) override;
    void buttonStateChanged (juce::Button* button) override;
private:
    FunkitAudioProcessor& _processor;
    juce::Label _label {"Snare", "Snare Drum"};
    LabeledSlider _noteSlider;
    LabeledSlider _levelSlider;
    LabeledSlider _decaySlider;
    LabeledSlider _shapeSlider;
    LabeledSlider _driveSlider;
    LabeledSlider _noiseSlider;
    LabeledSlider _reverbSlider;
    LabeledSlider _reverbSizeSlider;
    LabeledSlider _gateSlider;
    
    juce::TextButton _trigger;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SnareComponent)
};
