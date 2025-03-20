/*
  ==============================================================================

    Wood.cpp
    Created: 17 Mar 2025 2:57:15pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#include "Wood.h"

Wood::Wood(WoodParameters& parameters, int octave) : _params(parameters), _octave(octave) {
    _adsr.setParameters(_adsrParams);
    _c1 = new FMOperator(0.5f, 1.0f, FMSignalFunction::sin);
    _c2 = new FMOperator(2.0f, 1.0f, FMSignalFunction::sin);
    _m1 = new FMOperator(3.25f, 1.0f, FMSignalFunction::sin);
    _m2 = new FMOperator(0.5f, 1.0f, FMSignalFunction::sin);
    
    _c1->addModulator(_m1);
    _c2->addModulator(_m2);
}

Wood::~Wood() {
    delete _c1;
    delete _c2;
}

void Wood::prepareToPlay (double sampleRate, int samplesPerBlock, int numOutputChannels) {
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numOutputChannels;
    
    _c1->prepare(spec);
    _c2->prepare(spec);
    
    _adsr.setSampleRate(sampleRate);
    
    _gain.prepare(spec);
    
    _filter.setMode(juce::dsp::LadderFilterMode::LPF24);
    _filter.setCutoffFrequencyHz(16000.0f);
    _filter.setDrive(1.0f);
    _filter.setResonance(0.0f);
    _filter.prepare(spec);
    _filter.setEnabled(true);
    
    _reverbParameters.width = .2;
    _reverbParameters.dryLevel = .7;
    _reverbParameters.wetLevel = .3;
    _reverbParameters.damping = .4;
    _reverbParameters.roomSize = .6;
    _reverbParameters.freezeMode = 0.0;
    _reverb.setParameters(_reverbParameters);
    _reverb.setSampleRate(sampleRate);
    
    _isPrepared = true;
}

bool Wood::canPlaySound (juce::SynthesiserSound *) {
    return true;
}

void Wood::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) {
    if(midiNoteNumber == 43) {
        float frequency = std::abs(juce::MidiMessage::getMidiNoteInHertz(_params.getNote()));
        if(_octave > 0) {
            frequency *= _octave*2.0;
        }
        if(_octave < 0) {
            frequency /= _octave*-2.0;
        }
        _frequency = frequency;
        _c1->setFrequency(frequency);
        _c2->setFrequency(frequency);
        _c1->reset();
        _c2->reset();
        _adsr.noteOn();
        _reverb.reset();
    }
}

void Wood::stopNote (float velocity, bool allowTailOff) {
    _adsr.noteOff();
}

void Wood::controllerMoved (int controllerNumber, int newControllerValue) {
    
}

void Wood::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) {
    if(! isVoiceActive()) {
        return;
    }
    
    jassert(_isPrepared);
    
    setUpParameters();
    
    _synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    _synthBuffer.clear();
    
    juce::dsp::AudioBlock<float> audioBlock { _synthBuffer };
    audioBlock.clear();
    
    float shape = _params.getShape();
    for(int s = 0; s < numSamples; ++s) {
        float envelope = _adsr.getNextSample();
        _c1->setFrequency(_frequency);
        _c2->setFrequency(_frequency);
        envelope = std::pow(envelope, shape);
        float c1Sample = _c1->nextSample(1.0f);
        float c2Sample = _c2->nextSample(1.0f);

        for(int c = 0; c < outputBuffer.getNumChannels(); ++c) {
            audioBlock.setSample(c, s, (c1Sample+c2Sample)*envelope);
        }
    }
    
    _filter.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});
    _reverb.processStereo(audioBlock.getChannelPointer(0), audioBlock.getChannelPointer(1), numSamples);
    _gain.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});

    for(int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
        outputBuffer.addFrom(channel, startSample, _synthBuffer, channel, 0, numSamples);
    }
}

void Wood::pitchWheelMoved (int newPitchWheelValue) {
    
}

void Wood::setUpParameters() {
    _adsrParams.decay = _params.getDecay();
    _adsr.setParameters(_adsrParams);
    _gain.setGainLinear(_params.getLevel());
    _filter.setCutoffFrequencyHz(_params.getCutoff());
    _m1->setRatio(_params.getRatioM1());
    _m2->setRatio(_params.getRatioM2());
    _reverbParameters.roomSize = _params.getReverbSize();
    _reverbParameters.wetLevel = _params.getReverb();
    _reverbParameters.dryLevel = 1-_params.getReverb();
    _reverb.setParameters(_reverbParameters);
}

WoodParameters::WoodParameters(juce::AudioProcessorValueTreeState& apvts) : _apvts(apvts){
    
}

WoodParameters::~WoodParameters() {
    
}

float WoodParameters::getDecay() {
    return _apvts.getRawParameterValue("WOOD_DECAY")->load();
}

int WoodParameters::getNote() {
    return _apvts.getRawParameterValue("WOOD_NOTE")->load();
}

float WoodParameters::getLevel() {
    return _apvts.getRawParameterValue("WOOD_LEVEL")->load();
}

float WoodParameters::getCutoff() {
    return _apvts.getRawParameterValue("WOOD_CUTOFF")->load();
}

float WoodParameters::getShape() {
    return _apvts.getRawParameterValue("WOOD_SHAPE")->load();
}

float WoodParameters::getRatioM1() {
    return _apvts.getRawParameterValue("WOOD_FM_RATIO_M1")->load();
}

float WoodParameters::getRatioM2() {
    return _apvts.getRawParameterValue("WOOD_FM_RATIO_M2")->load();
}

float WoodParameters::getReverb() {
    return _apvts.getRawParameterValue("WOOD_REVERB")->load();
}

float WoodParameters::getReverbSize() {
    return _apvts.getRawParameterValue("WOOD_REVERB_SIZE")->load();
}

std::vector<std::unique_ptr<juce::RangedAudioParameter>> WoodParameters::getParameters() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("WOOD_DECAY", 1), "Wood Decay", juce::NormalisableRange<float> {0.01f, 0.5f, 0.01f}, 0.25f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("WOOD_SHAPE", 1), "Wood Shape", juce::NormalisableRange<float> {1.0f, 5.0f, 0.1f}, 1.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("WOOD_LEVEL", 1), "Wood Level", juce::NormalisableRange<float> {0.00f, 1.0f, 0.01f, 0.4f}, 0.3f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("WOOD_CUTOFF", 1), "Wood Filter Cutoff", juce::NormalisableRange<float> {10.00f, 7500.0f, 10.0f, .3f}, 7500.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("WOOD_FM_RATIO_M1", 1), "Wood FM Ratio M1", juce::NormalisableRange<float> {0.5f, 16.0f, 0.5f}, 5.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("WOOD_FM_RATIO_M2", 1), "Wood FM Ratio M2", juce::NormalisableRange<float> {0.5f, 16.0f, 0.5f}, 0.5f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("WOOD_REVERB", 1), "Wood Reverb", juce::NormalisableRange<float> {0.00f, 1.0f, 0.01f}, 0.3f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("WOOD_REVERB_SIZE", 1), "Wood Reverb Size", juce::NormalisableRange<float> {0.00f, 1.0f, 0.01f}, 0.3f));
    
    params.push_back(std::make_unique<juce::AudioParameterInt>(juce::ParameterID("WOOD_NOTE", 1), "Wood Note", 60, 72, 57));
    
    return params;
}
