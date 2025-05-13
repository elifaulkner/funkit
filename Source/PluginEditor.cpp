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
    setSize (1000, 500);
    addAndMakeVisible(_kick);
    addAndMakeVisible(_snare);
    addAndMakeVisible(_hiHat);
    addAndMakeVisible(_wood);
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
    auto area = getLocalBounds();
    
    int height = 100;
        
    _kick.setBounds(area.removeFromTop(height));
    _snare.setBounds(area.removeFromTop(height));
    _hiHat.setBounds(area.removeFromTop(height));
    _wood.setBounds(area.removeFromTop(height));
    _global.setBounds(area.removeFromTop(height));
}
