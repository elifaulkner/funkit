/*
  ==============================================================================

    Delay.cpp
    Created: 13 Feb 2025 1:12:42pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#include "Delay.h"

Delay::Delay() {
    
}

Delay::~Delay() {
    
}

void Delay::setDelay(float delay) {
    _delay.setDelay(delay);
}

void Delay::setDelayLevel(float level) {
    _mixer.setWetMixProportion(level);
}

void Delay::prepare(juce::dsp::ProcessSpec &spec) {
    _delay.prepare(spec);
    _mixer.prepare(spec);
    _delay.setMaximumDelayInSamples(spec.sampleRate);
    _mixer.setMixingRule(juce::dsp::DryWetMixingRule::balanced);
    _mixer.setWetMixProportion(.5f);
}
