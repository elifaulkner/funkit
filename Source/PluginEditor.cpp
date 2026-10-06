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
    setLookAndFeel(&_lookAndFeel);
    setLookAndFeel(&_lookAndFeel);
    setSize (1000, 500);
    addAndMakeVisible(_kick);
    addAndMakeVisible(_snare);
    addAndMakeVisible(_hiHat);
    addAndMakeVisible(_wood);
    addAndMakeVisible(_global);
}

FunkitAudioProcessorEditor::~FunkitAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void FunkitAudioProcessorEditor::paint (juce::Graphics& g)
{
    FunkitLookAndFeel::paintBackground(g, getLocalBounds());
}

void FunkitAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    
    int height = getHeight() / 5;
        
    _kick.setBounds(area.removeFromTop(height));
    _snare.setBounds(area.removeFromTop(height));
    _hiHat.setBounds(area.removeFromTop(height));
    _wood.setBounds(area.removeFromTop(height));
    _global.setBounds(area.removeFromTop(height));
}
