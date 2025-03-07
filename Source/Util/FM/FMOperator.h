/*
  ==============================================================================

    FMOperator.h
    Created: 6 Mar 2025 3:08:38pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FMOperator {
    public:
    FMOperator(float ratio, float amplitude);
    ~FMOperator();
    void setFrequency(float frequency);
    void setRatio(float ratio);
    void setAmplitude(float amplitude);
    void addModulator(FMOperator* modulator);
    void prepare(juce::dsp::ProcessSpec& spec);
    float getSample(int sampleIndex);
    private:
    juce::dsp::ProcessSpec _spec;
    juce::dsp::Phase<float> _phase;
    std::list<FMOperator*> _modulators = {};
    double _frequency = 0;
    double _ratio = 0;
    double _amplitude 1.0f;
};
