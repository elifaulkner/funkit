/*
  ==============================================================================

    Saturator.h
    Created: 27 Feb 2025 8:43:52am
    Author:  Eli Faulkner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Saturator {
    public:
    Saturator();
    ~Saturator();
    void setSaturationLevel(float level);
    void prepare(juce::dsp::ProcessSpec& spec);
    template <typename ProcessContext> void process (const ProcessContext& context) noexcept
    {
        auto&& block = context.getInputBlock();
        _mixer.pushDrySamples(block);
        _shaper.process(context);
        _mixer.mixWetSamples(context.getOutputBlock());
    }

    private:
    juce::dsp::WaveShaper<float> _shaper;
    juce::dsp::DryWetMixer<float> _mixer;
};
