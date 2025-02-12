/*
  ==============================================================================

    DrumOscillator.cpp
    Created: 11 Feb 2025 3:49:07pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#include "DrumOscillator.h"

void DrumOscillator::prepare(juce::dsp::ProcessSpec& spec) {
    _spec = spec;
    _phase.reset();
}

void DrumOscillator::process (const juce::dsp::ProcessContextReplacing<float>& context) noexcept {
    float baseIncrement = juce::MathConstants<float>::twoPi / _spec.sampleRate;
    juce::dsp::AudioBlock<float> outputBuffer = context.getOutputBlock();
    
    for(int s = 0; s < outputBuffer.getNumSamples(); ++s) {
        float envelopeValue = std::max(0.0f, (_envelopeSamples-_envelopeCounter)/(_envelopeSamples));
        envelopeValue = std::pow(envelopeValue, _decayShape);
        _envelopeCounter++;
        float value = std::sin(_phase.advance(baseIncrement*_frequency*envelopeValue)-juce::MathConstants<float>::pi)*envelopeValue*_waveValue;
        
        for(int c = 0; c < outputBuffer.getNumChannels(); ++c) {
            outputBuffer.setSample(c, s, value+(_random.nextFloat()*_noiseLevel-_noiseLevel/2)*envelopeValue);
        }
    }
}

void DrumOscillator::noteOn() {
    _envelopeCounter = 0;
    _envelopeSamples = _decay*_spec.sampleRate;
}

void DrumOscillator::noteOff() {
    
}

void DrumOscillator::reset() {
    _envelopeCounter = 0;
}

void DrumOscillator::setFrequency(float frequency)
{
    _frequency = std::max(1.0f, frequency);
}

void DrumOscillator::setDecay(float decay)
{
    _decay = decay;
}

void DrumOscillator::setNoiseLevel(float noiseLevel) {
    _noiseLevel = noiseLevel;
}

void DrumOscillator::setWaveValue(float value) {
    _waveValue = value;
}
