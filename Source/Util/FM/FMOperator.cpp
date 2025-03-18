/*
  ==============================================================================

    FMOperator.cpp
    Created: 6 Mar 2025 3:08:38pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#include "FMOperator.h"

FMOperator::FMOperator(float ratio, float amplitude, FMSignalFunction function): _ratio(ratio), _amplitude(amplitude) {
    _signal.setFunction(function);
}

FMOperator::FMOperator(FMOperator& copy) {
    _ratio = copy._ratio;
    _amplitude = copy._amplitude;
    _frequency = copy._frequency;
    _signal = copy._signal;
    _spec = copy._spec;
    for(auto m : copy._modulators) {
        _modulators.push_back(new FMOperator(*m));
    }
}

FMOperator& FMOperator::operator=(const FMOperator& copy) {
    _ratio = copy._ratio;
    _amplitude = copy._amplitude;
    _frequency = copy._frequency;
    _signal = copy._signal;
    _spec = copy._spec;
    for(auto m : copy._modulators) {
        _modulators.push_back(new FMOperator(*m));
    }
    return *this;
}

FMOperator::~FMOperator() {
    while(!_modulators.empty()) {
        delete _modulators.front();
        _modulators.pop_front();
    }
}

void FMOperator::setAmplitude(float amplitude) {
    jassert(amplitude >= 0.0f);
    jassert(amplitude <= 1.0f);
    _amplitude = amplitude;
}

void FMOperator::reset() {
    _phase.reset();
}

void FMOperator::setRatio(float ratio) {
    _ratio = ratio;
}

void FMOperator::setFrequency(float frequency) {
    _frequency = frequency;
}

void FMOperator::setSignal(FMSignalFunction function) {
    _signal.setFunction(function);
}

void FMOperator::addModulator(FMOperator* modulator) {
    _modulators.push_back(modulator);
}

void FMOperator::prepare(juce::dsp::ProcessSpec& spec) {
    _spec = spec;
    _phase.reset();
    for(auto m : _modulators) {
        m->prepare(spec);
    }
}

float FMOperator::nextSample(float pitchEnvelope) {
    float baseIncrement = juce::MathConstants<float>::twoPi / _spec.sampleRate;
    float fm = 0.0f;
    for(auto m : _modulators) {
        fm += m->nextSample(pitchEnvelope);
    }
    float val = _signal.eval(_phase.advance(baseIncrement*_frequency*_ratio*pitchEnvelope)+fm-juce::MathConstants<float>::pi)*_amplitude;
    
    if(pitchEnvelope > 0.1) {
        //std::cout << "Operator: " << "Val " << val << " FM " << fm << std::endl;
    }
    
    return val;
}
