/*
  ==============================================================================

    HiHat.h
    Created: 12 Feb 2025 11:20:32am
    Author:  Eli Faulkner

  ==============================================================================
*/

#pragma once

#include "../Util/DrumOscillator.h"
#include "../Util/GlobalEffects.h"

class HiHatParameters {
    public:
    HiHatParameters(juce::AudioProcessorValueTreeState& apvts);
    ~HiHatParameters();
    static std::vector<std::unique_ptr<juce::RangedAudioParameter>> getParameters();
    float getDecay();
    float getLevel();
    float getDrive();
    float getNoiseLevel();
    float getShape();
    int getNote();
    bool useGlobal();
    private:

    juce::AudioProcessorValueTreeState& _apvts;
};

class HiHat : public juce::SynthesiserVoice {
    public:
    HiHat(GlobalEffects& global, HiHatParameters& parameters, int octave = 0);
    ~HiHat();
    bool canPlaySound (juce::SynthesiserSound *) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue)  override;
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> createParams();
    void prepareToPlay (double sampleRate, int samplesPerBlock, int numOutputChannels);

    private:
    bool _isPrepared = false;

    DrumOscillator _drum;
    
    juce::AudioBuffer<float> _synthBuffer;
    
    GlobalEffects& _global;
    
    juce::dsp::Gain<float> _gain;
    juce::dsp::LadderFilter<float> _filter;
    juce::dsp::Limiter<float> _limiter;
    juce::dsp::Compressor<float> _compressor;
    HiHatParameters& _params;
    int _octave = 0;
    
    void setUpParameters();
};
