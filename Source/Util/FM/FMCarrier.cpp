/*
  ==============================================================================

    FMCarrier.cpp
    Created: 6 Mar 2025 3:26:21pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#include "FMCarrier.h"

FMCarrier::FMCarrier(float ratio, float ampliture, FMSignalFunction function) : FMOperator(ratio, ampliture, function){
    
}

FMCarrier::~FMCarrier() {
    for(auto m : _modulators) {
        delete m;
    }
}

void FMCarrier::prepare(juce::dsp::ProcessSpec& spec) {
    _spec = spec;
    for(auto m : _modulators) {
        m->prepare(spec);
    }
}

void FMCarrier::setFrequency(float frequency) {
    _frequency = frequency;
    for(auto m : _modulators) {
        m->setFrequency(frequency);
    }
}
void FMCarrier::reset() {
    _phase.reset();
    for(auto m : _modulators) {
        m->reset();
    }
}

float FMCarrier::nextSample(float pitchEnvelope) {
    float baseIncrement = juce::MathConstants<float>::twoPi / _spec.sampleRate;

    float fm = 0.0f;
    for(auto m : _modulators) {
        fm += m->nextSample(pitchEnvelope);
    }
    return _signal.eval(_phase.advance(baseIncrement*_frequency*pitchEnvelope)+fm-juce::MathConstants<float>::pi);
}
