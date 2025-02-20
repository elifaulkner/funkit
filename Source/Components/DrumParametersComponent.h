/*
  ==============================================================================

    DrumParametersComponent.h
    Created: 20 Feb 2025 1:49:32pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "KickComponent.h"
#include "SnareComponent.h"
#include "HiHatComponent.h"
#include "GlobalEffectsComponent.h"

//==============================================================================
/*
*/
class DrumParametersComponent  : public juce::Component
{
public:
    DrumParametersComponent(FunkitAudioProcessor&);
    ~DrumParametersComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    FunkitAudioProcessor& _audioProcessor;

    KickComponent _kick;
    SnareComponent _snare;
    HiHatComponent _hiHat;
    GlobalEffectsComponent _global;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DrumParametersComponent)
};
