/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Drums/Kick.h"
#include "Drums/Snare.h"
#include "Drums/HiHat.h"
#include "SynthSound.h"

//==============================================================================
/**
*/
class FunkitAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    FunkitAudioProcessor();
    ~FunkitAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getApvts();
    
    void triggerKick(float velocity);
    void triggerSnare(float velocity);
    void triggerHiHat(float velocity);
private:
    juce::AudioProcessorValueTreeState _apvts;
    
    juce::Synthesiser _kickSynth;
    KickParameters _kickParameters;

    juce::Synthesiser _snareSynth;
    SnareParameters _snareParameters;
    
    juce::Synthesiser _hiHatSynth;
    HiHatParameters _hiHatParameters;

    GlobalEffectsParameters _globalParams;
    GlobalEffects _global;

    
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FunkitAudioProcessor)
};
