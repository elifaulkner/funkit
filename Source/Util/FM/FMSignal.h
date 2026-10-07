/*
  ==============================================================================

    FMSignal.h
    Created: 7 Mar 2025 9:44:09am
    Author:  Eli Faulkner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

enum FMSignalFunction {sine, square, noise, saw, triangle};

class FMSignal {
    public:
    FMSignal();
    ~FMSignal();
    FMSignal(FMSignal& copy);
    FMSignal& operator=(const FMSignal& other);
    void setFunction(FMSignalFunction function);
    float eval(float x);
    private:
    FMSignalFunction _function = FMSignalFunction::sine;
    juce::Random* _random;
};
