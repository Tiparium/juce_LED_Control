/*
  ==============================================================================

    MainComponent.cpp
    Created: 26 Sep 2022 11:09:01am
    Author:  Tiparium

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

MainComponent::~MainComponent()
{
}

void MainComponent::paint (juce::Graphics& g)
{
    setBoundsRelative(0.35f, 0.35f, 0.5f, 0.5f);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
    g.fillAll (juce::Colours::darkgoldenrod);

    g.setColour (juce::Colours::grey);



    g.setColour (juce::Colours::white);
    g.setFont (30.0f);
    g.drawText (currentSizeAsString, getLocalBounds(),
                juce::Justification::centred, true);
}

void MainComponent::resized()
{
    setBoundsRelative(0.35f, 0.35f, 0.3f, 0.3f);
    currentSizeAsString = juce::String (getWidth()) + " x " + juce::String (getHeight());
}
