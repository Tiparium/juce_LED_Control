/*
  ==============================================================================

    FavoritesBar.cpp
    Created: 13 Oct 2022 4:44:29pm
    Author:  Tiparium

  ==============================================================================
*/

#include "FavoritesBar.h"

FavoritesBar::FavoritesBar(RestHandler& RestHandler) {
    _restHandler = RestHandler;

    // Trying a vector of favSlots to be iterated over
    _favSlots.push_back(&_testSlot);
    _testSlot.addChangeListener(this);
}

FavoritesBar::~FavoritesBar() {
    _testSlot.removeChangeListener(this);
}

void FavoritesBar::changeListenerCallback(juce::ChangeBroadcaster* src) {

    // TODO: Set this to an actual thing later. Right now this will just print some stuff to show it worked.
    _restHandler.getPhilipsData();
    juce::uint8 r = -1;
    juce::uint8 g = -1;
    juce::uint8 b = -1;

    // TODO MAKE THIS WORK.
    for (unsigned int i = 0; i < _favSlots.size(); i++) {
        juce::String debugStr = "Did Not Work :(";
        if (src == *&_favSlots[i]) {
            r = _favSlots[i]->getR();
            g = _favSlots[i]->getG();
            b = _favSlots[i]->getB();
            juce::String rBit = "R: " + r;
            juce::String gBit = " G: " + g;
            juce::String bBit = " B: " + b;
            debugStr = rBit + gBit + bBit;
        }
        DBG(debugStr);
    }
}

void FavoritesBar::paint(juce::Graphics& g) {
    // Fill in background color
    g.drawRect(getLocalBounds(), 1);
    g.fillAll(juce::Colours::darkorange);

    // Try drawing a temp FavoritesSlot
    addAndMakeVisible(_testSlot);
    _testSlot.setBoundsOnScreen();
}

void FavoritesBar::setBoundsOnScreen() {
    setBoundsRelative(0.0f, 0.0f, 1.0f, 0.2f);
}