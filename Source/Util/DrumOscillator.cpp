/*
  ==============================================================================

    DrumOscillator.cpp
    Created: 11 Feb 2025 3:49:07pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#include "DrumOscillator.h"

DrumOscillator::DrumOscillator(FMCarrier& carrier, FMCarrier& impactCarrier) : _carrier(carrier), _impactCarrier(impactCarrier) {
    
}

DrumOscillator::~DrumOscillator() {
}

void DrumOscillator::prepare(juce::dsp::ProcessSpec& spec) {
    _spec = spec;
    _carrier.prepare(spec);
    _impactCarrier.prepare(spec);
}

void DrumOscillator::noteOn() {
    _envelopeCounter = 0;
    _impactEnvelopeCounter = 0;
    _envelopeSamples = _decay*_spec.sampleRate;
    _impactEnvelopeSamples = _impactDecay*_spec.sampleRate;
    _carrier.reset();
    _impactCarrier.reset();
}

void DrumOscillator::noteOff() {
    
}

void DrumOscillator::reset() {
    _envelopeCounter = 0;
    _impactEnvelopeCounter = 0;
}

void DrumOscillator::setFrequency(float frequency)
{
    _carrier.setFrequency(frequency);
    _impactCarrier.setFrequency(frequency);
}

void DrumOscillator::setDecay(float decay)
{
    _decay = decay;
}

void DrumOscillator::setDecayShape(float shape) {
    _decayShape = shape;
}

void DrumOscillator::setVelocity(float velocity) {
    _velocity = velocity;
}
