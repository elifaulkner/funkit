/*
  ==============================================================================

    DrumOscillator.h
    Created: 11 Feb 2025 3:49:07pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#include <JuceHeader.h>

#pragma once

class DrumOscillator {
    public:
    void prepare(juce::dsp::ProcessSpec& spec);
    void process (const juce::dsp::ProcessContextReplacing<float>& context) noexcept;
    void noteOn();
    void noteOff();
    void setFrequency(float frequency);
    void setDecay(float decay);
    void setNoiseLevel(float noiseLevel);
    void setWaveValue(float value);
    void reset();
    
    private:
    juce::dsp::ProcessSpec _spec;
    juce::dsp::Phase<float> _phase;
    float _envelopeValue = 0.0;
    float _decay = 0.25;
    float _decayShape = 2.0;
    float _frequency = 10.0f;
    float _envelopeCounter = 0.0;
    int _envelopeSamples = 0.0;
    float _waveValue = 1.0;
    float _noiseLevel = 0.0;
    juce::Random _random;
};
