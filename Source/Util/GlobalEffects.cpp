/*
  ==============================================================================

    GlobalEffects.cpp
    Created: 13 Feb 2025 10:18:56am
    Author:  Eli Faulkner
  ==============================================================================
*/

#include "GlobalEffects.h"

GlobalEffects::GlobalEffects(GlobalEffectsParameters& params) : _params(params){
    
}

GlobalEffects::~GlobalEffects() {
    
}

void GlobalEffects::prepare(juce::dsp::ProcessSpec& spec) {
    if(!_isPrepared) {
        _delay.prepare(spec);
        
        _filter.prepare(spec);
        _filter.setMode(juce::dsp::LadderFilterMode::LPF24);
        _filter.setDrive(1.0f);
        _filter.setResonance(0.0f);
        _filter.setCutoffFrequencyHz(20000.0f);
        
        _sampleRate = spec.sampleRate;
        _isPrepared = true;
    }
}

void GlobalEffects::setUpParameters() {
    _delay.setDelay(_params.getDelay()*_sampleRate);
    _delay.setDelayLevel(_params.getDelayLevel());
    _delay.setFeedback(_params.getDelayFeedback());
    _filter.setCutoffFrequencyHz(_params.getCutoff());
    _filter.setResonance(_params.getResonance());
}

GlobalEffectsParameters::GlobalEffectsParameters(juce::AudioProcessorValueTreeState& apvts) : _apvts(apvts){
    
}

GlobalEffectsParameters::~GlobalEffectsParameters() {
    
}

float GlobalEffectsParameters::getDelay() {
    return _apvts.getRawParameterValue("GLOBAL_DELAY")->load();
}

float GlobalEffectsParameters::getCutoff() {
    return _apvts.getRawParameterValue("GLOBAL_CUTOFF")->load();
}

float GlobalEffectsParameters::getResonance() {
    return _apvts.getRawParameterValue("GLOBAL_RES")->load();
}

float GlobalEffectsParameters::getDelayLevel() {
    return _apvts.getRawParameterValue("GLOBAL_DELAY_LEVEL")->load();
}

float GlobalEffectsParameters::getDelayFeedback() {
    return _apvts.getRawParameterValue("GLOBAL_DELAY_FEEDBACK")->load();
}

std::vector<std::unique_ptr<juce::RangedAudioParameter>> GlobalEffectsParameters::getParameters() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("GLOBAL_DELAY", 1), "Delay", juce::NormalisableRange<float> {0.00f, 1.0f, 0.01f}, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("GLOBAL_DELAY_LEVEL", 1), "Delay Level", juce::NormalisableRange<float> {0.0f, 1.0f, 0.01f}, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterInt>(juce::ParameterID("GLOBAL_DELAY_FEEDBACK", 1), "Delay Feedback", 1, 10, 1));

    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("GLOBAL_CUTOFF", 1), "Cutoff", juce::NormalisableRange<float> {1.00f, 20000.0f, 1.0f}, 20000.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("GLOBAL_RES", 1), "Resonance", juce::NormalisableRange<float> {0.00f, 1.0f, 0.01f}, 0.0f));
    
    return params;
}
