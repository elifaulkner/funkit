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
    _c1 = new FMOperator("Wood Carrier", 1.0f, 1.0f, FMSignalFunction::sine);
    _m1 = new FMOperator("Wood M1", 2.3, 1.0f, FMSignalFunction::sine);
    _m2 = new FMOperator("Wood M2", 1.7, 0.5f, FMSignalFunction::sine);
    
    _m1->addModulator(_m2);
    _c1->addModulator(_m1);
}

Wood::~Wood() {
    delete _c1;
}

void Wood::prepareToPlay (double sampleRate, int samplesPerBlock, int numOutputChannels) {
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numOutputChannels;
    
    _c1->prepare(spec);
    
    _adsr.setSampleRate(sampleRate);
    _indexCoef = std::exp(-1.0f / (0.02f * (float) sampleRate));
    _pitchCoef = std::exp(-1.0f / (0.01f * (float) sampleRate));
    _clickCoef = std::exp(-1.0f / (0.008f * (float) sampleRate));
    
    
    _gain.prepare(spec);
    
    _filter.setMode(juce::dsp::LadderFilterMode::LPF24);
    _filter.setCutoffFrequencyHz(12000.0f);
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
        frequency *= std::pow(2.0f, (float) _octave);
        _frequency = frequency;
        _c1->setFrequency(frequency);
        _c1->reset();
        _indexEnv = 1.0f;
        _pitchEnv = 1.05f;
        _clickEnv = 1.0f;
        _clickLast = 0.0f;
        _adsr.noteOn();
    } else {
        clearCurrentNote();
    }
}

void Wood::stopNote (float velocity, bool allowTailOff) {
    _adsr.noteOff();
    if(! allowTailOff) {
        _adsr.reset();
        clearCurrentNote();
    }
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
        envelope = std::pow(envelope, shape);

        _m1->setAmplitude(_indexEnv);
        _m2->setAmplitude(_indexEnv * 0.5f);
        float c1Sample = _c1->nextSample(_pitchEnv);
        _indexEnv *= _indexCoef;
        _pitchEnv = 1.0f + (_pitchEnv - 1.0f) * _pitchCoef;

        // Short high-passed noise burst for the click
        float noise = _random.nextFloat() * 2.0f - 1.0f;
        float highNoise = noise - _clickLast;
        _clickLast = noise;
        float click = highNoise * 0.5f * _clickEnv * _clickLevel;
        _clickEnv *= _clickCoef;

        float out = c1Sample * envelope + click;
        for(int c = 0; c < outputBuffer.getNumChannels(); ++c) {
            audioBlock.setSample(c, s, out);
        }
    }
    
    _filter.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});
    if(_synthBuffer.getNumChannels() >= 2) {
        _reverb.processStereo(audioBlock.getChannelPointer(0), audioBlock.getChannelPointer(1), numSamples);
    } else {
        _reverb.processMono(audioBlock.getChannelPointer(0), numSamples);
    }
    _gain.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});

    for(int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
        outputBuffer.addFrom(channel, startSample, _synthBuffer, channel, 0, numSamples);
    }

    if(! _adsr.isActive()) {
        clearCurrentNote();
    }
}

void Wood::pitchWheelMoved (int newPitchWheelValue) {
    
}

void Wood::setUpParameters() {
    _adsrParams.attack = 0.001f;
    _adsrParams.decay = _params.getDecay();
    _adsrParams.sustain = 0.0f;
    _adsrParams.release = 0.02f;
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

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("WOOD_DECAY", 1), "Wood Decay", juce::NormalisableRange<float> {0.01f, 0.5f, 0.01f}, 0.1f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("WOOD_SHAPE", 1), "Wood Shape", juce::NormalisableRange<float> {1.0f, 5.0f, 0.1f}, 1.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("WOOD_LEVEL", 1), "Wood Level", juce::NormalisableRange<float> {0.00f, 1.0f, 0.01f, 0.4f}, 0.3f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("WOOD_CUTOFF", 1), "Wood Filter Cutoff", juce::NormalisableRange<float> {10.00f, 16000.0f, 10.0f, .3f}, 12000.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("WOOD_FM_RATIO_M1", 1), "Wood FM Ratio M1", juce::NormalisableRange<float> {0.5f, 8.0f, 0.01f}, 2.3f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("WOOD_FM_RATIO_M2", 1), "Wood FM Ratio M2", juce::NormalisableRange<float> {0.5f, 8.0f, 0.01f}, 1.7f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("WOOD_REVERB", 1), "Wood Reverb", juce::NormalisableRange<float> {0.00f, 1.0f, 0.01f}, 0.3f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("WOOD_REVERB_SIZE", 1), "Wood Reverb Size", juce::NormalisableRange<float> {0.00f, 1.0f, 0.01f}, 0.3f));
    
    params.push_back(std::make_unique<juce::AudioParameterInt>(juce::ParameterID("WOOD_NOTE", 1), "Wood Note", 48, 84, 72));
    
    return params;
}
