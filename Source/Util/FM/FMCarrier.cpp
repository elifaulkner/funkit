/*
  ==============================================================================

    FMCarrier.cpp
    Created: 6 Mar 2025 3:26:21pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#include "FMCarrier.h"

FMCarrier::FMCarrier() {
    
}

FMCarrier::~FMCarrier() {
    for(auto m : _modulators) {
        delete m;
    }
}


void FMCarrier::addModulator(FMOperator* modulator) {
    _modulators.push_back(modulator);
}

void FMCarrier::prepare(juce::dsp::ProcessSpec& spec) {
    _spec = spec;
}

void FMCarrier::setFrequency(float frequency) {
    _frequency = frequency;
    for(auto m : _modulators) {
        m->setFrequency(frequency);
    }
}
