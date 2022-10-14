/*
  ==============================================================================

    Main_C.cpp
    Created: 9 Oct 2022 12:05:31am
    Author:  Tiparium

    Contains & Displays subcomponents both for data processing & UI display.

  ==============================================================================
*/


#include "Main_C.h"


Main_C::Main_C() :
    _restHandler(),
    _RGBSliders(_restHandler),
    _FavoritesBar(_restHandler)
{
}

Main_C::~Main_C() {

}

void Main_C::paint(juce::Graphics& g) {

    // Create and draw all three RGB sliders as a subcomponent
    g.drawRect(getLocalBounds(), 1);
    g.fillAll(juce::Colours::red);
    addAndMakeVisible(_RGBSliders);
    _RGBSliders.setBounds(0, 0, juce::Component::getHeight(), juce::Component::getWidth());

    // Create & draw Favirotes Bar
    g.drawRect(getLocalBounds(), 1);
    g.fillAll(juce::Colours::blue);
    addAndMakeVisible(_FavoritesBar);
    _FavoritesBar.setBounds(0, 0, juce::Component::getHeight(), juce::Component::getWidth());

}

void Main_C::resized() {

}