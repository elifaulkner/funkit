/*
  ==============================================================================

    SequencerComponent.cpp
    Created: 20 Feb 2025 1:48:24pm
    Author:  Eli Faulkner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SequencerComponent.h"

//==============================================================================
SequencerComponent::SequencerComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

SequencerComponent::~SequencerComponent()
{
}

void SequencerComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("SequencerComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void SequencerComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
