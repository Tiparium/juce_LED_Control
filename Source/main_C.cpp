/*
  ==============================================================================

    main_C.cpp
    Created: 9 Oct 2022 12:05:31am
    Author:  Tiparium

  ==============================================================================
*/


#include "main_C.h"


Main_C::Main_C() :
    _restHandler(),
    _rgbSliders(_restHandler)
{
}

Main_C::~Main_C() {

}

void Main_C::paint(juce::Graphics& g) {

    // Create and draw all three RGB sliders as a subcomponent
    g.drawRect(getLocalBounds(), 1);
    g.fillAll(juce::Colours::red);
    addAndMakeVisible(_rgbSliders);
    _rgbSliders.setBounds(0, 0, juce::Component::getHeight(), juce::Component::getWidth());

}

void Main_C::resized() {

}