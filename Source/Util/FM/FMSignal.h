/*
  ==============================================================================

    FMSignal.h
    Created: 7 Mar 2025 9:44:09am
    Author:  Eli Faulkner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

enum FMSignalFunction {sin, square, noise};

class FMSignal {
    public:
    void setFunction(FMSignalFunction function);
    float eval(float x);
    private:
    FMSignalFunction _function = FMSignalFunction::sin;
    juce::Random _random;
};
