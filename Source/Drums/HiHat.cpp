/*
  ==============================================================================

    HiHat.cpp
    Created: 12 Feb 2025 11:20:32am
    Author:  Eli Faulkner

  ==============================================================================
*/

#include "HiHat.h"

HiHat::HiHat(HiHatParameters& parameters, int octave) :
    _params(parameters),
    _octave(octave) {
    
}

HiHat::~HiHat() {
    
}

void HiHat::prepareToPlay (double sampleRate, int samplesPerBlock, int numOutputChannels) {
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numOutputChannels;
    
    _drum.prepare(spec);
    _drum.setDecay(_params.getDecay());
    _drum.setNoiseLevel(_params.getNoiseLevel());
    _drum.setWaveValue(0);
    
    _gain.prepare(spec);
    _gain.setGainLinear(0.5f);
    
    _filter.prepare(spec);
    
    _filter.setEnabled(true);
    _filter.setMode(juce::dsp::LadderFilterMode::LPF24);
    _filter.setDrive(1.0f);
    _filter.setResonance(0.0f);
    _filter.setCutoffFrequencyHz(500.0f);
    
    _compressor.prepare(spec);
    _compressor.setAttack(10);
    _compressor.setRelease(25);
    _compressor.setRatio(8);
    _compressor.setThreshold(-15);
    
    _limiter.prepare(spec);
    _limiter.setThreshold(-2.0);
    _limiter.setRelease(25);
    
    _isPrepared = true;
}

void HiHat::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) {
    if(! isVoiceActive()) {
        return;
    }
    
    jassert(_isPrepared);
    
    setUpParameters();
    
    _synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    _synthBuffer.clear();
    
    juce::dsp::AudioBlock<float> audioBlock { _synthBuffer };
    audioBlock.clear();
    
    _drum.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
        
    _filter.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    
    _gain.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});

    //_compressor.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    
    _limiter.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});
    
    for(int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
        outputBuffer.addFrom(channel, startSample, _synthBuffer, channel, 0, numSamples);
    }
}

void HiHat::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) {
    if(midiNoteNumber == 42) {
        float frequency = std::abs(juce::MidiMessage::getMidiNoteInHertz(42));
        if(_octave > 0) {
            frequency *= _octave*2.0;
        }
        if(_octave < 0) {
            frequency /= _octave*-2.0;
        }
        _drum.setFrequency(frequency);
        _drum.noteOn();
    }
}

void HiHat::stopNote (float velocity, bool allowTailOff) {
    _drum.noteOff();
    if(! allowTailOff ) {
        clearCurrentNote();
    }
}

bool HiHat::canPlaySound (juce::SynthesiserSound *sound) {
    return true;
}

void HiHat::controllerMoved (int controllerNumber, int newControllerValue) {
    
}

void HiHat::pitchWheelMoved (int newPitchWheelValue) {

}

std::vector<std::unique_ptr<juce::RangedAudioParameter>> HiHatParameters::getParameters() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("HIHAT_DECAY", "Decay", juce::NormalisableRange<float> {0.01f, 1.0f, 0.01f}, 0.15f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("HIHAT_LEVEL", "Level", juce::NormalisableRange<float> {0.00f, 1.0f, 0.01f}, 0.8f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("HIHAT_DRIVE", "Drive", juce::NormalisableRange<float> {1.00f, 10.0f, 1.0f}, 1.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("HIHAT_NOISE", "Noise", juce::NormalisableRange<float> {0.00f, 1.0f, 0.01f}, 0.8f));

    return params;
}

void HiHat::setUpParameters() {
    _drum.setDecay(_params.getDecay());
    _drum.setNoiseLevel(_params.getNoiseLevel());
    _gain.setGainLinear(_params.getLevel());
    _filter.setDrive(_params.getDrive());
}

HiHatParameters::HiHatParameters(juce::AudioProcessorValueTreeState& apvts) : _apvts(apvts) {
    
}

HiHatParameters::~HiHatParameters() {
    
}

float HiHatParameters::getDecay() {
    return _apvts.getRawParameterValue("HIHAT_DECAY")->load();
}
 
float HiHatParameters::getLevel() {
    return _apvts.getRawParameterValue("HIHAT_LEVEL")->load();
}

float HiHatParameters::getDrive() {
    return _apvts.getRawParameterValue("HIHAT_DRIVE")->load();
}

float HiHatParameters::getNoiseLevel() {
    return _apvts.getRawParameterValue("HIHAT_NOISE")->load();
}
