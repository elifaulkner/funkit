/*
  ==============================================================================

    Snare.h
    Created: 6 Feb 2025 2:10:26pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Util/DrumOscillator.h"

class SnareParameters {
    public:
    SnareParameters(juce::AudioProcessorValueTreeState& apvts);
    ~SnareParameters();
    static std::vector<std::unique_ptr<juce::RangedAudioParameter>> getParameters();
    float getDecay();
    float getLevel();
    float getDrive();
    float getNoiseLevel();
    float getReverb();
    float getReverbSize();
    int getNote();
    float getNoiseGateThreshold();
    private:

    juce::AudioProcessorValueTreeState& _apvts;
};

class Snare : public juce::SynthesiserVoice {
    public:
    Snare(SnareParameters& parameters, int octave);
    ~Snare();
    bool canPlaySound (juce::SynthesiserSound *) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue)  override;
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    
    void prepareToPlay (double sampleRate, int samplesPerBlock, int numOutputChannels);
    
    private:
    bool _isPrepared = false;
    
    DrumOscillator _drum;
    
    juce::AudioBuffer<float> _synthBuffer;
    
    juce::dsp::Gain<float> _gain;
    juce::dsp::LadderFilter<float> _filter;
    juce::Random _random;
    juce::dsp::Limiter<float> _limiter;
    juce::dsp::Compressor<float> _compressor;
    juce::dsp::NoiseGate<float> _gate;
    juce::dsp::Reverb _reverb;
    juce::dsp::Reverb::Parameters _reverbParameters;
    int _octave = 0;
    juce::dsp::ProcessSpec _spec;
    
    SnareParameters& _params;
    
    void setUpParameters();
};
