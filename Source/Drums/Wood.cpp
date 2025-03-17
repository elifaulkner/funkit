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
    _c1 = new FMCarrier(7.0f, 1.0f, FMSignalFunction::sin);
    _c2 = new FMCarrier(12.11f, 1.0f, FMSignalFunction::sin);
    _m1 = new FMOperator(5.0f, 1.0f, FMSignalFunction::sin);
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
    _m1->prepare(spec);
    _m2->prepare(spec);
    
    _adsr.setSampleRate(sampleRate);
    
    _gain.prepare(spec);
    
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
        _c1->setFrequency(frequency);
        _c2->setFrequency(frequency);
        _c1->reset();
        _c2->reset();
        _adsr.noteOn();
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
    
    for(int s = 0; s < numSamples; ++s) {
        float c1Sample = _c1->nextSample(1.0f);
        float c2Sample = _c2->nextSample(1.0f);
        float envelope = _adsr.getNextSample();
        
        for(int c = 0; c < outputBuffer.getNumChannels(); ++c) {
            audioBlock.setSample(c, s, (c1Sample+c2Sample)*envelope);
        }
    }
    
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

std::vector<std::unique_ptr<juce::RangedAudioParameter>> WoodParameters::getParameters() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("WOOD_DECAY", 1), "Wood Decay", juce::NormalisableRange<float> {0.01f, 0.5f, 0.01f}, 0.25f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("WOOD_LEVEL", 1), "Wood Level", juce::NormalisableRange<float> {0.00f, 1.0f, 0.01f, 0.4f}, 0.3f));
    
    params.push_back(std::make_unique<juce::AudioParameterInt>(juce::ParameterID("WOOD_NOTE", 1), "Wood Note", 36, 48, 45));
    
    return params;
}
