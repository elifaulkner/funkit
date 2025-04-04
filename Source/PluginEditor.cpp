/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FunkitAudioProcessorEditor::FunkitAudioProcessorEditor (FunkitAudioProcessor& p)
: AudioProcessorEditor (&p), audioProcessor (p), _kick(p, p.getApvts()), _snare(p, p.getApvts()), _hiHat(p, p.getApvts()), _wood(p, p.getApvts()) ,_global(p, p.getApvts())
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1000, 400);
    addAndMakeVisible(_kick);
    addAndMakeVisible(_snare);
    addAndMakeVisible(_hiHat);
    //addAndMakeVisible(_wood);
    addAndMakeVisible(_global);
}

FunkitAudioProcessorEditor::~FunkitAudioProcessorEditor()
{
}

//==============================================================================
void FunkitAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::grey);
}

void FunkitAudioProcessorEditor::resized()
{
    _kick.setBounds(0, 0, 1000, 100);
    _snare.setBounds(0, 100, 1000, 100);
    _hiHat.setBounds(0, 200, 1000, 100);
    //_wood.setBounds(0, 300, 1000, 100);
    _global.setBounds(0, 300, 1000, 100);
}
