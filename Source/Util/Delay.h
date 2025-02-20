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
    void setFeedback(int feedback);
    void prepare(juce::dsp::ProcessSpec& spec);
    template <typename ProcessContext> void process (const ProcessContext& context) noexcept
    {
        for(int k = 0; k < _feedback; k++) {
            _mixer[k].reset();
            _mixer[k].pushDrySamples(context.getInputBlock());
            _delay[k].process(context);
            _mixer[k].mixWetSamples(context.getOutputBlock());
        }
    }
    private:
    std::array<juce::dsp::DelayLine<float>, 10> _delay;
    std::array<juce::dsp::DryWetMixer<float>, 10> _mixer;
    float _sampleRate = 0;
    int _feedback = 1;
};
