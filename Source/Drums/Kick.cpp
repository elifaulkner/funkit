/*
  ==============================================================================

    Kick.cpp
    Created: 6 Feb 2025 10:44:59am
    Author:  Eli Faulkner

  ==============================================================================
*/

#include "Kick.h"

Kick::Kick(KickParameters& parameters, int octave) : _params(parameters), _octave(octave){
}

Kick::~Kick() {
    
}

void Kick::prepareToPlay (double sampleRate, int samplesPerBlock, int numOutputChannels) {
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numOutputChannels;
    
    _drum.prepare(spec);
    _drum.setDecay(_params.getDecay());
    _drum.setNoiseLevel(_params.getNoiseLevel());
    
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

bool Kick::canPlaySound (juce::SynthesiserSound * sound) {
    return true;
}

void Kick::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
    if(midiNoteNumber == 36) {
        float frequency = std::abs(juce::MidiMessage::getMidiNoteInHertz(_params.getNote()));
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

void Kick::stopNote (float velocity, bool allowTailOff)
{
    _drum.noteOff();
    if(! allowTailOff ) {
        clearCurrentNote();
    }
}

void Kick::controllerMoved (int controllerNumber, int newControllerValue)
{
    
}

void Kick::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
{
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

    _compressor.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    
    _limiter.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});
    
    for(int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
        outputBuffer.addFrom(channel, startSample, _synthBuffer, channel, 0, numSamples);
    }
}

void Kick::setUpParameters() {
    _drum.setDecay(_params.getDecay());
    _drum.setDecayShape(_params.getShape());
    _gain.setGainLinear(_params.getLevel());
    _filter.setDrive(_params.getDrive());
}

void Kick::pitchWheelMoved (int newPitchWheelValue)
{
    
}

KickParameters::KickParameters(juce::AudioProcessorValueTreeState& apvts) : _apvts(apvts)
{
}

KickParameters::~KickParameters()
{
    
}

float KickParameters::getDecay() {
    return _apvts.getRawParameterValue("KICK_DECAY")->load();
}

float KickParameters::getShape() {
    return _apvts.getRawParameterValue("KICK_SHAPE")->load();
}


float KickParameters::getLevel() {
    return _apvts.getRawParameterValue("KICK_LEVEL")->load();
}

float KickParameters::getDrive() {
    return _apvts.getRawParameterValue("KICK_DRIVE")->load();
}

float KickParameters::getNoiseLevel() {
    return _apvts.getRawParameterValue("KICK_NOISE")->load();
}

int KickParameters::getNote() {
    return _apvts.getRawParameterValue("KICK_NOTE")->load();
}

std::vector<std::unique_ptr<juce::RangedAudioParameter>> KickParameters::getParameters() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("KICK_DECAY", "Decay", juce::NormalisableRange<float> {0.01f, 1.0f, 0.01f}, 0.25f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("KICK_SHAPE", "Shape", juce::NormalisableRange<float> {1.0f, 5.0f, 0.1f}, 1.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("KICK_LEVEL", "Level", juce::NormalisableRange<float> {0.00f, 1.0f, 0.01f}, 0.5f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("KICK_DRIVE", "Drive", juce::NormalisableRange<float> {1.00f, 10.0f, 1.0f}, 1.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("KICK_NOISE", "Noise", juce::NormalisableRange<float> {0.00f, 0.5f, 0.01f}, 0.125f));

    params.push_back(std::make_unique<juce::AudioParameterInt>("KICK_NOTE", "Kick Note", 24, 36, 33));
    
    return params;
}
