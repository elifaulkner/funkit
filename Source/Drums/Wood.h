/*
  ==============================================================================

    Wood.h
    Created: 17 Mar 2025 2:57:15pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Util/DrumOscillator.h"
#include "../Util/GlobalEffects.h"

class WoodParameters {
    public:
    WoodParameters(juce::AudioProcessorValueTreeState& apvts);
    ~WoodParameters();
    int getNote();
    float getDecay();
    float getShape();
    float getLevel();
    float getCutoff();
    float getReverbSize();
    float getReverb();
    float getRatioM1();
    float getRatioM2();
    
    static std::vector<std::unique_ptr<juce::RangedAudioParameter>> getParameters();
    private:
    juce::AudioProcessorValueTreeState& _apvts;
};

class Wood : public juce::SynthesiserVoice {
    public:
    Wood(WoodParameters& parameters, int octave = 0);
    ~Wood();
    bool canPlaySound (juce::SynthesiserSound *) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue)  override;
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    
    void prepareToPlay (double sampleRate, int samplesPerBlock, int numOutputChannels);
    private:
    bool _isPrepared = false;

    FMOperator* _c1;
    FMOperator* _m1;
    FMOperator* _m2;
    
    juce::ADSR _adsr;
    juce::ADSR::Parameters _adsrParams {0.05, 0.2f, 0.0f, 0.1f};

    juce::AudioBuffer<float> _synthBuffer;

    juce::dsp::Gain<float> _gain;
    juce::dsp::LadderFilter<float> _filter;
    juce::Reverb _reverb;
    juce::Reverb::Parameters _reverbParameters;
    
    WoodParameters& _params;
    int _octave = 0;
    float _frequency = 0.0f;
    
    void setUpParameters();
};
