/*
  ==============================================================================

    DrumOscillator.h
    Created: 11 Feb 2025 3:49:07pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FM/FMHeader.h"

#pragma once

class DrumOscillator {
    public:
    DrumOscillator(FMCarrier* carrier, FMCarrier* impactCarrier);
    ~DrumOscillator();
    void prepare(juce::dsp::ProcessSpec& spec);
    template <typename ProcessContext> void process (const ProcessContext& context) noexcept
    {
        juce::dsp::AudioBlock<float> outputBuffer = context.getOutputBlock();
        
        for(int s = 0; s < outputBuffer.getNumSamples(); ++s) {
            float envelopeValue = std::max(0.0f, (_envelopeSamples-_envelopeCounter)/(_envelopeSamples));
            envelopeValue = std::pow(envelopeValue, _decayShape);
            _envelopeCounter++;
            
            float impactEnvelopeValue = std::max(0.0f, (_impactEnvelopeSamples-_impactEnvelopeCounter)/(_impactEnvelopeSamples));
            impactEnvelopeValue = std::pow(impactEnvelopeValue, 2.0f);
            _impactEnvelopeCounter++;
            
            float impactValue = _impactCarrier->nextSample(_pitchEnvelope? impactEnvelopeValue : 1.0f);
            float value = _carrier->nextSample(_pitchEnvelope? envelopeValue : 1.0f);

            for(int c = 0; c < outputBuffer.getNumChannels(); ++c) {
                outputBuffer.setSample(c, s, value*envelopeValue+impactValue*impactEnvelopeValue);
            }

            float adjVelocity = std::pow(_velocity, 1.25);
            for(int c = 0; c < outputBuffer.getNumChannels(); ++c) {
                outputBuffer.setSample(c, s, outputBuffer.getSample(c, s)*adjVelocity);
            }
        }
    }
    void noteOn();
    void noteOff();
    void setFrequency(float frequency);
    void setDecay(float decay);
    void setImpactDecay(float decay);
    void setDecayShape(float shape);
    void setVelocity(float velocity);
    void setPitchEnvelope(bool pitchEnvelope);
    void reset();
    
    private:
    FMCarrier* _carrier;
    FMCarrier* _impactCarrier;
    juce::dsp::ProcessSpec _spec;
    FMOperator* _noiseOperator;
    float _envelopeValue = 0.0;
    float _decay = 0.25;
    float _decayShape = 2.0;
    float _envelopeCounter = 0.0;
    int _envelopeSamples = 0.0;
    bool _pitchEnvelope = true;
    
    float _impactEnvelopeValue = 0.0;
    float _impactDecay = 0.075;
    float _impactEnvelopeCounter = 0.0;
    int _impactEnvelopeSamples = 0.0;

    float _velocity = 1.0f;
};
