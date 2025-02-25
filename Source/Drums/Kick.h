/*
  ==============================================================================

    Kick.h
    Created: 6 Feb 2025 10:44:59am
    Author:  Eli Faulkner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "../Util/DrumOscillator.h"
#include "../Util/GlobalEffects.h"

class KickParameters {
    public:
    KickParameters(juce::AudioProcessorValueTreeState& apvts);
    ~KickParameters();
    static std::vector<std::unique_ptr<juce::RangedAudioParameter>> getParameters();
    float getDecay();
    float getLevel();
    float getDrive();
    float getNoiseLevel();
    float getShape();
    int getNote();
    private:

    juce::AudioProcessorValueTreeState& _apvts;
};

class Kick : public juce::SynthesiserVoice {
    public:
    Kick(GlobalEffects& global, KickParameters& parameters, int octave = 0);
    ~Kick();
    bool canPlaySound (juce::SynthesiserSound *) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue)  override;
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    
    void prepareToPlay (double sampleRate, int samplesPerBlock, int numOutputChannels);
    
    //void prepareToPlay(juce::dsp::ProcessSpec& spec);

    private:
    bool _isPrepared = false;

    DrumOscillator _drum;
    
    juce::AudioBuffer<float> _synthBuffer;
    
    juce::dsp::Gain<float> _gain;
    juce::dsp::LadderFilter<float> _filter;
    juce::dsp::Limiter<float> _limiter;
    juce::dsp::Compressor<float> _compressor;
    
    
    KickParameters& _params;
    int _octave = 0;
    
    void setUpParameters();
};


#pragma once
