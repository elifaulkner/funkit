/*
  ==============================================================================

    FMCarrier.h
    Created: 6 Mar 2025 3:26:21pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FMOperator.h"

class FMCarrier : public FMOperator {
    public:
    FMCarrier(float ratio = 1.0f, float amplitude = 1.0f, FMSignalFunction function = FMSignalFunction::sin);
    FMCarrier(FMCarrier& copy);
    ~FMCarrier();
    FMCarrier& operator=(const FMCarrier& other);
    void prepare(juce::dsp::ProcessSpec& spec);
    void setFrequency(float frequency);
    float nextSample(float pitchEnvelope);
    void reset();
    private:
};
