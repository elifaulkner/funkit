/*
  ==============================================================================

    FMSignal.cpp
    Created: 7 Mar 2025 9:44:09am
    Author:  Eli Faulkner

  ==============================================================================
*/

#include "FMSignal.h"

FMSignal::FMSignal() {
    _random = new juce::Random();
    std::cout << "New FMSignal" << std::endl;
}

FMSignal::~FMSignal() {
    delete _random;
    std::cout << "Delete FM Signal" << std::endl;
}

FMSignal::FMSignal(FMSignal& copy) {
    _function = copy._function;
    _random = new juce::Random();
    
    std::cout << "Copy FM Signal" << std::endl;
}

FMSignal& FMSignal::operator=(const FMSignal& other) {
    _function = other._function;
    std::cout << "Assign FM Signal" << std::endl;
    
    return *this;
}

void FMSignal::setFunction(FMSignalFunction function) {
    _function = function;
}

float FMSignal::eval(float x) {
    switch(_function) {
        case FMSignalFunction::sin:
            return std::sin(x);
        case FMSignalFunction::noise:
            return _random->nextDouble()*2-1;
        case FMSignalFunction::square:
            return x < 0 ? -1 : 1;
        case FMSignalFunction::saw:
            return x/juce::MathConstants<float>::pi;
        default:
            return 0;
    }
}
