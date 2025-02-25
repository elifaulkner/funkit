/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FunkitAudioProcessor::FunkitAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), _apvts(*this, nullptr, "Parameters", createParams())
                        ,_kickParameters(_apvts),
                        _snareParameters(_apvts),
                        _hiHatParameters(_apvts),
                        _globalParams(_apvts),
                        _global(_globalParams)
#endif
{
    _kickSynth.addSound(new SynthSound());
    _kickSynth.addVoice(new Kick(_global, _kickParameters, 0));
    _kickSynth.addVoice(new Kick(_global, _kickParameters, 0));

    _snareSynth.addSound(new SynthSound());
    _snareSynth.addVoice(new Snare(_global, _snareParameters, 0));
    _snareSynth.addVoice(new Snare(_global, _snareParameters, 0));
    
    _hiHatSynth.addSound(new SynthSound());
    _hiHatSynth.addVoice(new HiHat(_global, _hiHatParameters , 0));
    _hiHatSynth.addVoice(new HiHat(_global, _hiHatParameters, 0));
}

FunkitAudioProcessor::~FunkitAudioProcessor()
{
}

//==============================================================================
const juce::String FunkitAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FunkitAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FunkitAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FunkitAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FunkitAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FunkitAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FunkitAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FunkitAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FunkitAudioProcessor::getProgramName (int index)
{
    return "Default";
}

void FunkitAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FunkitAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();

    _global.prepare(spec);
    _kickSynth.setCurrentPlaybackSampleRate(sampleRate);
    _snareSynth.setCurrentPlaybackSampleRate(sampleRate);
    _hiHatSynth.setCurrentPlaybackSampleRate(sampleRate);    

    
    for(int i = 0; i < _kickSynth.getNumVoices(); i++) {
        if(auto voice = dynamic_cast<Kick*>(_kickSynth.getVoice(i))) {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }

    for(int i = 0; i < _snareSynth.getNumVoices(); i++) {
        if(auto voice = dynamic_cast<Snare*>(_snareSynth.getVoice(i))) {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }

    for(int i = 0; i < _hiHatSynth.getNumVoices(); i++) {
        if(auto voice = dynamic_cast<HiHat*>(_hiHatSynth.getVoice(i))) {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void FunkitAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FunkitAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FunkitAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for(int i = 0; i < _kickSynth.getNumVoices(); ++i) {
        if(auto kick = dynamic_cast<Kick*>(_kickSynth.getVoice(i))) {
            // Kick parameters
        }
    }

    for(int i = 0; i < _snareSynth.getNumVoices(); ++i) {
        if(auto snare = dynamic_cast<Snare*>(_snareSynth.getVoice(i))) {
            // Snare parameters
        }
    }
    
    for(int i = 0; i < _hiHatSynth.getNumVoices(); ++i) {
        if(auto snare = dynamic_cast<Snare*>(_hiHatSynth.getVoice(i))) {
            // Snare parameters
        }
    }
    
    _kickSynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    _snareSynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    _hiHatSynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
    juce::dsp::AudioBlock<float> audioBlock { buffer };
    //audioBlock.clear();
    
    _global.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});
}

//==============================================================================
bool FunkitAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FunkitAudioProcessor::createEditor()
{
    return new FunkitAudioProcessorEditor (*this);
}

//==============================================================================
void FunkitAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = _apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void FunkitAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (_apvts.state.getType()))
            _apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FunkitAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout FunkitAudioProcessor::createParams() {
    // ComboBox switch osc
    
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    for(std::unique_ptr<juce::RangedAudioParameter>& p : KickParameters::getParameters()) {
        params.push_back(std::move(p));
    }
    
    for(std::unique_ptr<juce::RangedAudioParameter>& p : SnareParameters::getParameters()) {
        params.push_back(std::move(p));
    }

    for(std::unique_ptr<juce::RangedAudioParameter>& p : HiHatParameters::getParameters()) {
        params.push_back(std::move(p));
    }

    for(std::unique_ptr<juce::RangedAudioParameter>& p : GlobalEffectsParameters::getParameters()) {
        params.push_back(std::move(p));
    }
    
    return {params.begin(), params.end()};
}


juce::AudioProcessorValueTreeState& FunkitAudioProcessor::getApvts() {
    return _apvts;
}

void FunkitAudioProcessor::triggerKick(float velocity)
{
    _kickSynth.noteOn(0, 36, velocity);
}

void FunkitAudioProcessor::triggerSnare(float velocity)
{
    _snareSynth.noteOn(0, 38, velocity);
}

void FunkitAudioProcessor::triggerHiHat(float velocity)
{
    _hiHatSynth.noteOn(0, 42, velocity);
}
