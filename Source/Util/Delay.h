/*
  ==============================================================================

    Delay.h
    Created: 13 Feb 2025 1:12:42pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Delay {
    public:
    Delay();
    ~Delay();
    void setDelay(float delay);
    void setDelayLevel(float level);
    void prepare(juce::dsp::ProcessSpec& spec);
    template <typename ProcessContext> void process (const ProcessContext& context) noexcept
    {
        _mixer.reset();
        _mixer.pushDrySamples(context.getInputBlock());
        _delay.process(context);
        _mixer.mixWetSamples(context.getOutputBlock());
    }
    private:
    juce::dsp::DelayLine<float> _delay;
    juce::dsp::DryWetMixer<float> _mixer;
    float _sampleRate = 0;
};
