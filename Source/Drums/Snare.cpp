/*
  ==============================================================================

    Snare.cpp
    Created: 6 Feb 2025 2:10:26pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#include "Snare.h"

Snare::Snare(GlobalEffects& global, SnareParameters& params, int octave) :
_drum(_carrier, _impactCarrier), _octave(octave), _params(params)
{
    _noiseOperator = new FMOperator("Snare Noise", 1.0f, 0.3f, FMSignalFunction::noise);
    _op1 = new FMOperator("Snare Modulator", 2.0f, 1.0f, FMSignalFunction::sin);
    _carrier->addModulator(_op1);
    _carrier->addModulator(_noiseOperator);
}

Snare::~Snare() {
    
}

void Snare::prepareToPlay (double sampleRate, int samplesPerBlock, int numOutputChannels) {
    _spec.maximumBlockSize = samplesPerBlock;
    _spec.sampleRate = sampleRate;
    _spec.numChannels = numOutputChannels;
    
    _drum.prepare(_spec);
    _drum.setDecay(_params.getDecay());
    
    _gain.prepare(_spec);
    _gain.setGainLinear(0.4f);
    
    _filter.prepare(_spec);
    
    _filter.setEnabled(true);
    _filter.setMode(juce::dsp::LadderFilterMode::HPF12);
    _filter.setDrive(1.0f);
    _filter.setResonance(0.0f);
    _filter.setCutoffFrequencyHz(150.0f);
    
    _compressor.prepare(_spec);
    _compressor.setAttack(10);
    _compressor.setRelease(25);
    _compressor.setRatio(8);
    _compressor.setThreshold(-15);
    
    _limiter.prepare(_spec);
    _limiter.setThreshold(-2.0);
    _limiter.setRelease(25);
    
    _gate.prepare(_spec);
    _gate.setAttack(0.05);
    _gate.setRelease(0.1);
    _gate.setThreshold(-25.0f);
    _gate.setRatio(8.0f);
    
    _reverbParameters.width = .2;
    _reverbParameters.dryLevel = .7;
    _reverbParameters.wetLevel = .3;
    _reverbParameters.damping = .4;
    _reverbParameters.roomSize = .6;
    _reverbParameters.freezeMode = 0.0;
    _reverb.setParameters(_reverbParameters);
    _reverb.prepare(_spec);
        
    _isPrepared = true;
}

bool Snare::canPlaySound (juce::SynthesiserSound * sound) {
    return true;
}

void Snare::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
    if(midiNoteNumber == 38) {
        float frequency = std::abs(juce::MidiMessage::getMidiNoteInHertz(_params.getNote()));
        if(_octave > 0) {
            frequency *= _octave*2.0;
        }
        if(_octave < 0) {
            frequency /= _octave*-2.0;
        }
        
        _drum.setFrequency(frequency);
        _drum.setVelocity(velocity);
        _drum.noteOn();
    }
}

void Snare::stopNote (float velocity, bool allowTailOff)
{
    _drum.noteOff();
    
    if(! allowTailOff) {
        clearCurrentNote();
    }
}

void Snare::controllerMoved (int controllerNumber, int newControllerValue)
{
    
}

void Snare::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
{
    jassert(_isPrepared);

    if(! isVoiceActive()) {
        return;
    }

    setUpParameters();
    
    _synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    _synthBuffer.clear();
    
    juce::dsp::AudioBlock<float> audioBlock { _synthBuffer };

    audioBlock.clear();
    
    _drum.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    
    _filter.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    _compressor.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    
    _gate.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    
    _reverb.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});
    
    _gain.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});

    _limiter.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});
    
    for(int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
        outputBuffer.addFrom(channel, startSample, _synthBuffer, channel, 0, numSamples);
    }
}

void Snare::pitchWheelMoved (int newPitchWheelValue)
{
    
}

void Snare::setUpParameters() {
    _drum.setDecay(_params.getDecay());
    _noiseOperator->setAmplitude(_params.getNoiseLevel());
    _op1->setAmplitude(_params.getFMAmount());
    _drum.setDecayShape(_params.getShape());
    _gate.setThreshold(_params.getNoiseGateThreshold());
    _gain.setGainLinear(_params.getLevel());
    _filter.setDrive(_params.getDrive());
    _reverbParameters.wetLevel = _params.getReverb();
    _reverbParameters.dryLevel = 1-_params.getReverb();
    _reverbParameters.roomSize = _params.getReverbSize();
    _reverb.setParameters(_reverbParameters);
    _drum.setImpactDecay(_params.getImpact());
}

SnareParameters::SnareParameters(juce::AudioProcessorValueTreeState& apvts): _apvts(apvts){
    
}

SnareParameters::~SnareParameters() {
    
}

std::vector<std::unique_ptr<juce::RangedAudioParameter>> SnareParameters::getParameters() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("SNARE_DECAY", 1), "Snare Decay", juce::NormalisableRange<float> {0.01f, 0.5f, 0.01f}, 0.25f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("SNARE_SHAPE", 1), "Snare Shape", juce::NormalisableRange<float> {1.0f, 5.0f, 0.1f}, 1.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("SNARE_LEVEL", 1), "Snare Level", juce::NormalisableRange<float> {0.00f, 1.0f, 0.01f, 0.4f}, 0.3f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("SNARE_DRIVE", 1), "Snare Drive", juce::NormalisableRange<float> {1.00f, 10.0f, 1.0f}, 2.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("SNARE_NOISE", 1), "Snare Noise", juce::NormalisableRange<float> {0.00f, 0.25f, 0.00001f, 0.2f}, 0.01f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("SNARE_REVERB", 1), "Snare Reverb", juce::NormalisableRange<float> {0.00f, 1.0f, 0.01f}, 0.3f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("SNARE_REVERB_SIZE", 1), "Snare Reverb Size", juce::NormalisableRange<float> {0.00f, 1.0f, 0.01f}, 0.3f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("SNARE_GATE_THRESHOLD", 1), "Snare Gate Threshold", juce::NormalisableRange<float> {-50.0f, -1.0f, 0.1f, 0.3}, -40.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterInt>(juce::ParameterID("SNARE_NOTE", 1), "Snare Note", 36, 48, 45));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("SNARE_IMPACT", 1), "Snare Impact", juce::NormalisableRange<float> {0.0001f, 1.0f, 0.0001f, 0.25f}, 0.05f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("SNARE_FM", 1), "Snare FM", juce::NormalisableRange<float> {0.0f, 1.0f, 0.001f, 0.25f}, 0.5f));
    
    return params;
}

float SnareParameters::getDecay() {
    return _apvts.getRawParameterValue("SNARE_DECAY")->load();
}

float SnareParameters::getShape() {
    return _apvts.getRawParameterValue("SNARE_SHAPE")->load();
}

float SnareParameters::getLevel() {
    return _apvts.getRawParameterValue("SNARE_LEVEL")->load();
}

float SnareParameters::getDrive() {
    return _apvts.getRawParameterValue("SNARE_DRIVE")->load();
}

float SnareParameters::getNoiseLevel() {
    return _apvts.getRawParameterValue("SNARE_NOISE")->load();
}

float SnareParameters::getReverb() {
    return _apvts.getRawParameterValue("SNARE_REVERB")->load();
}

float SnareParameters::getReverbSize() {
    return _apvts.getRawParameterValue("SNARE_REVERB_SIZE")->load();
}

int SnareParameters::getNote() {
    return _apvts.getRawParameterValue("SNARE_NOTE")->load();
}

float SnareParameters::getNoiseGateThreshold() {
    return _apvts.getRawParameterValue("SNARE_GATE_THRESHOLD")->load();
}

float SnareParameters::getImpact() {
    return _apvts.getRawParameterValue("SNARE_IMPACT")->load();
}

float SnareParameters::getFMAmount() {
    return _apvts.getRawParameterValue("SNARE_FM")->load();
}
