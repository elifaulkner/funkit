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

FMCarrier::FMCarrier(FMCarrier& copy) : FMOperator(copy) {

}

FMCarrier& FMCarrier::operator=(const FMCarrier& copy) {
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

FMCarrier::~FMCarrier() {
    while(!_modulators.empty()) {
        delete _modulators.front();
        _modulators.pop_front();
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
    float val = _signal.eval(_phase.advance(baseIncrement*_frequency*pitchEnvelope)+fm-juce::MathConstants<float>::pi)*_amplitude;
    
    if(pitchEnvelope > 0.1) {
        //std::cout << "Carrier: " << "Val " << val << " FM " << fm << std::endl;
    }
    
    return val;
}
