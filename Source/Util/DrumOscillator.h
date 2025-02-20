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
    template <typename ProcessContext> void process (const ProcessContext& context) noexcept
    {
        float baseIncrement = juce::MathConstants<float>::twoPi / _spec.sampleRate;
        juce::dsp::AudioBlock<float> outputBuffer = context.getOutputBlock();
        
        for(int s = 0; s < outputBuffer.getNumSamples(); ++s) {
            float envelopeValue = std::max(0.0f, (_envelopeSamples-_envelopeCounter)/(_envelopeSamples));
            envelopeValue = std::pow(envelopeValue, _decayShape);
            _envelopeCounter++;
            float value = 0.0f;
            if(_useWave) {
                value = std::sin(_phase.advance(baseIncrement*_frequency*envelopeValue)-juce::MathConstants<float>::pi)*envelopeValue;
            }

            for(int c = 0; c < outputBuffer.getNumChannels(); ++c) {
                outputBuffer.setSample(c, s, value+(_random.nextFloat()*_noiseLevel-_noiseLevel/2)*envelopeValue);
            }

            for(int c = 0; c < outputBuffer.getNumChannels(); ++c) {
                outputBuffer.setSample(c, s, outputBuffer.getSample(c, s)*_velocity);
            }
        }
    }
    void noteOn();
    void noteOff();
    void setFrequency(float frequency);
    void setDecay(float decay);
    void setNoiseLevel(float noiseLevel);
    void setUseWave(bool value);
    void setDecayShape(float shape);
    void setVelocity(float velocity);
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
    bool _useWave = true;
    float _noiseLevel = 0.0;
    juce::Random _random;
    float _velocity = 1.0f;
};
