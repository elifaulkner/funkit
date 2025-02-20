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
    for(int k = 0; k < _delay.size(); k++) {
        _delay[k].setDelay(delay);
    }
}

void Delay::setDelayLevel(float level) {
    for(int k = 0; k < _mixer.size(); k++) {
        _mixer[k].setWetMixProportion(level/(1.0+k));
    }
}

void Delay::setFeedback(int feedback) {
    _feedback = std::min(feedback, (int)_delay.size());
    _feedback = std::max(_feedback, 1);
}

void Delay::prepare(juce::dsp::ProcessSpec &spec) {
    for(int k = 0; k < _delay.size(); k++) {
        _delay[k].prepare(spec);
        _mixer[k].prepare(spec);
        _delay[k].setMaximumDelayInSamples(spec.sampleRate);
        _mixer[k].setMixingRule(juce::dsp::DryWetMixingRule::balanced);
        _mixer[k].setWetMixProportion(.5f/(k+1.0f));
    }
}
