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

    // Create root rectangle
    g.drawRect(getLocalBounds(), 1);
    g.fillAll(juce::Colours::blue);

    // Create & draw Favorites Bar
    addAndMakeVisible(_FavoritesBar);
    _FavoritesBar.setBoundsOnScreen();

    // Create and draw all three RGB sliders
    addAndMakeVisible(_RGBSliders);
    _RGBSliders.setBoundsOnScreen();
}