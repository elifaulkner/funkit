/*
  ==============================================================================

    SynthSound.h
    Created: 6 Feb 2025 12:49:56pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
    public:
    bool appliesToNote (int midiNoteNumber) override { return true;}
    bool appliesToChannel (int midiChannel) override { return true;}
};
