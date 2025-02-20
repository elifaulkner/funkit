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

void DrumOscillator::setUseWave(bool value) {
    _useWave = value;
}

void DrumOscillator::setDecayShape(float shape) {
    _decayShape = shape;
}

void DrumOscillator::setVelocity(float velocity) {
    _velocity = velocity;
}
