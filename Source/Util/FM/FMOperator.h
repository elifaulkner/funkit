/*
  ==============================================================================

    FMOperator.h
    Created: 6 Mar 2025 3:08:38pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FMSignal.h"

class FMOperator {
    public:
    FMOperator(juce::String name, float ratio = 1.0f, float amplitude = 1.0f, FMSignalFunction function = FMSignalFunction::sin);
    FMOperator(FMOperator& copy);
    ~FMOperator();
    FMOperator& operator=(const FMOperator& other);
    void setFrequency(float frequency);
    void setRatio(float ratio);
    void setAmplitude(float amplitude);
    void setSignal(FMSignalFunction function);
    void addModulator(FMOperator* modulator);
    void prepare(juce::dsp::ProcessSpec& spec);
    float nextSample(float pitchEnvelope = 1.0f);
    void reset();
    protected:
    juce::dsp::ProcessSpec _spec;
    juce::dsp::Phase<float> _phase;
    std::list<FMOperator*> _modulators = {};
    juce::String _name;
    double _frequency = 0;
    double _ratio = 0;
    double _amplitude  = 1.0f;
    FMSignal* _signal;
    private:
};
