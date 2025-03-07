/*
  ==============================================================================

    FMCarrier.h
    Created: 6 Mar 2025 3:26:21pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FMOperator.h"

class FMCarrier {
    public:
    FMCarrier();
    ~FMCarrier();
    void addModulator(FMOperator* modulator);
    void setFrequency(float frequency);
    void prepare(juce::dsp::ProcessSpec& spec);
    template <typename ProcessContext> void process (const ProcessContext& context) noexcept
    {
        auto&& buffer = context.getOutputBlock();
        
        float baseIncrement = juce::MathConstants<float>::twoPi / _spec.sampleRate;

        for(int s = 0; s < buffer.getNumSamples(); ++s) {
            float fm = 0.0f;
            for(auto m : _modulators) {
                fm += m->getSample(s);
            }
            for(int c = 0; c < buffer.getNumChannels(); ++c) {
                buffer.setSample(c, s, std::sin(_phase.advance(baseIncrement*_frequency)+fm-juce::MathConstants<float>::pi));
            }
        }
    }
    private:
    juce::dsp::ProcessSpec _spec;
    juce::dsp::Phase<float> _phase;
    std::list<FMOperator*> _modulators = {};
    float _frequency = 0;
};
