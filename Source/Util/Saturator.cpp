/*
  ==============================================================================

    Saturator.cpp
    Created: 27 Feb 2025 8:43:52am
    Author:  Eli Faulkner

  ==============================================================================
*/

#include "Saturator.h"

Saturator::Saturator() {
    _shaper.functionToUse = [](float x) { return std::tanh(x); };
}

Saturator::~Saturator() {
    
}

void Saturator::prepare(juce::dsp::ProcessSpec& spec) {
    _shaper.prepare(spec);
    _mixer.prepare(spec);
    _mixer.setMixingRule(juce::dsp::DryWetMixingRule::balanced);
    _mixer.setWetMixProportion(.5f);
}

void Saturator::setSaturationLevel(float level) {
    _mixer.setWetMixProportion(level);
}
