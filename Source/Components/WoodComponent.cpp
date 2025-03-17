/*
  ==============================================================================

    WoodComponent.cpp
    Created: 17 Mar 2025 4:51:39pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WoodComponent.h"

//==============================================================================
WoodComponent::WoodComponent(FunkitAudioProcessor& ap, juce::AudioProcessorValueTreeState& apvts) :  _processor(ap),
_trigger("Trigger (G2)"),
_noteSlider("Tune", apvts, "WOOD_NOTE"),
_levelSlider("Level", apvts, "WOOD_LEVEL"),
_decaySlider("Decay", apvts, "WOOD_DECAY")
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(_trigger);
    addAndMakeVisible(_noteSlider);
    addAndMakeVisible(_levelSlider);
    addAndMakeVisible(_decaySlider);
    
    _trigger.addListener(this);
}

WoodComponent::~WoodComponent()
{
}

void WoodComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::grey);
}

void WoodComponent::resized()
{
    auto area = getLocalBounds();
    
    int labelHeight = 25;
    
    _label.setBounds(area.removeFromTop(labelHeight));
    
    int sliderWidth = 75;
    
    _noteSlider.setBounds(area.removeFromLeft(sliderWidth));
    _levelSlider.setBounds(area.removeFromLeft(sliderWidth));
    _decaySlider.setBounds(area.removeFromLeft(sliderWidth));
    
    _trigger.setBounds(850, 40, _trigger.getBestWidthForHeight(40), 40);

}

void WoodComponent::buttonClicked (juce::Button *button) {
}

void WoodComponent::buttonStateChanged (juce::Button* button) {
    if(button->isDown()) {
        _processor.triggerWood(1.0f);
    }
}
