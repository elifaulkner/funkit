/*
  ==============================================================================

    FMOperator.cpp
    Created: 6 Mar 2025 3:08:38pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#include "FMOperator.h"

FMOperator::FMOperator(juce::String name, float ratio, float amplitude, FMSignalFunction function): _ratio(ratio), _amplitude(amplitude) {
    _name = name;
    _signal = new FMSignal();
    _signal->setFunction(function);
}

FMOperator::FMOperator(FMOperator& copy) {
    _ratio = copy._ratio;
    _amplitude = copy._amplitude;
    _frequency = copy._frequency;
    _signal = new FMSignal(*copy._signal);
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
    delete _signal;
}

void FMOperator::setAmplitude(float amplitude) {
    jassert(amplitude >= 0.0f);
    jassert(amplitude <= 1.0f);
    _amplitude = amplitude;
}

void FMOperator::reset() {
    _phase.reset();
    for(auto m : _modulators) {
        m->reset();
    }
}

void FMOperator::setRatio(float ratio) {
    _ratio = ratio;
}

void FMOperator::setFrequency(float frequency) {
    _frequency = frequency;
    for(auto m : _modulators) {
        m->setFrequency(frequency);
    }
}

void FMOperator::setSignal(FMSignalFunction function) {
    _signal->setFunction(function);
}

void FMOperator::addModulator(FMOperator* modulator) {
    _modulators.push_back(modulator);
}

void FMOperator::prepare(juce::dsp::ProcessSpec& spec) {
    _spec = spec;
    _phase.reset();
    for(auto m : _modulators) {
        m->prepare(spec);
        m->reset();
    }
}

float FMOperator::nextSample(float pitchEnvelope) {
    float baseIncrement = juce::MathConstants<float>::twoPi / _spec.sampleRate;
    float fm = 0.0f;
    for(FMOperator *m : _modulators) {
        fm += m->nextSample(pitchEnvelope);
    }

    float val = _signal->eval(_phase.advance(baseIncrement*_frequency*_ratio*pitchEnvelope)+fm-juce::MathConstants<float>::pi)*_amplitude;

    return val;
}
