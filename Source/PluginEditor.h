/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Components/KickComponent.h"
#include "Components/SnareComponent.h"
#include "Components/HiHatComponent.h"
#include "Components/GlobalEffectsComponent.h"
//==============================================================================
/**
*/
class FunkitAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FunkitAudioProcessorEditor (FunkitAudioProcessor&);
    ~FunkitAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FunkitAudioProcessor& audioProcessor;

    KickComponent _kick;
    SnareComponent _snare;
    HiHatComponent _hiHat;
    GlobalEffectsComponent _global;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FunkitAudioProcessorEditor)
};
