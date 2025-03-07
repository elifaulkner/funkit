/*
  ==============================================================================

    FMOperator.cpp
    Created: 6 Mar 2025 3:08:38pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#include "FMOperator.h"

FMOperator::FMOperator(float ratio, float amplitude): _ratio(ratio), _amplitude(amplitude) {
}

FMOperator::~FMOperator() {

}

void FMOperator::setAmplitude(float amplitude) {
    jassert(amplitude >= 0.0f);
    jassert(amplitude <= 1.0f);
    _amplitude = amplitude;
}

void FMOperator::setRatio(float ratio) {
    _ratio = ratio;
}

void FMOperator::setFrequency(float frequency) {
    _frequency = frequency;
}

void FMOperator::addModulator(FMOperator* modulator) {
    _modulators.push_back(modulator);
}

void FMOperator::prepare(juce::dsp::ProcessSpec& spec) {
    _spec = spec;
    _phase.reset();
}

float FMOperator::getSample(int sampleIndex) {
    float baseIncrement = juce::MathConstants<float>::twoPi / _spec.sampleRate;
    float fm = 0.0f;
    for(auto m : _modulators) {
        fm += m->getSample(sampleIndex);
    }
    return std::sin(_phase.advance(baseIncrement*_frequency*_ratio)+fm-juce::MathConstants<float>::pi)*_amplitude;
}
