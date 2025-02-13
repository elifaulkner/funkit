/*
  ==============================================================================

    GlobalEffects.h
    Created: 13 Feb 2025 10:18:56am
    Author:  Eli Faulkner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Delay.h"

class GlobalEffectsParameters {
    public:
    GlobalEffectsParameters(juce::AudioProcessorValueTreeState& apvts);
    ~GlobalEffectsParameters();
    float getDelay();
    float getDelayLevel();
    float getCutoff();
    float getResonance();
    
    static std::vector<std::unique_ptr<juce::RangedAudioParameter>> getParameters();
    private:
    juce::AudioProcessorValueTreeState& _apvts;
};

class GlobalEffects {
    public:
    GlobalEffects(GlobalEffectsParameters& params);
    ~GlobalEffects();
    void prepare(juce::dsp::ProcessSpec& spec);
    template <typename ProcessContext> void process (const ProcessContext& context) noexcept
    {
        jassert(_isPrepared);
        setUpParameters();
        
        _delay.process(context);
        _filter.process(context);
    }
    private:
    bool _isPrepared = false;
    GlobalEffectsParameters& _params;
    
    Delay _delay;
    juce::dsp::LadderFilter<float> _filter;
    
    void setUpParameters();
};
