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
    FavoritesSlot* slot = new FavoritesSlot();
    FavoritesSlot* slotTwo = new FavoritesSlot();
    FavoritesSlot* slotThree = new FavoritesSlot();
    FavoritesSlot* slotFour = new FavoritesSlot();
    _favSlots.push_back(slot);
    _favSlots.push_back(slotTwo);
    _favSlots.push_back(slotThree);
    _favSlots.push_back(slotFour);

    slot->getButton().addListener(this);
    slotTwo->getButton().addListener(this);
    slotThree->getButton().addListener(this);
    slotFour->getButton().addListener(this);
}

FavoritesBar::~FavoritesBar() {
    for (unsigned int i = 0; i < _favSlots.size(); i++) {
        delete(_favSlots[i]);
    }
}

void FavoritesBar::buttonClicked(juce::Button* button) {
    RGB rgb;
    for (unsigned int i = 0; i < _favSlots.size(); i++) {
        if (button == &_favSlots[i]->getButton());
        rgb = _favSlots[i]->grabCurrentRGB();
    }
    DBG("Button Has Been Clicked!");
}

void FavoritesBar::paint(juce::Graphics& g) {
    // Fill in background color
    g.drawRect(getLocalBounds(), 1);
    g.fillAll(juce::Colours::darkorange);

    // Try drawing a temp FavoritesSlot
    int numSlots = _favSlots.size();
    float relativePos   = 0.0f;
    float relativeWidth = getWidth() / numSlots;
    for (unsigned int i = 0; i < _favSlots.size(); i++) {
        addAndMakeVisible(*&_favSlots[i]);
        _favSlots[i]->setBounds(relativePos, 0, relativeWidth, getHeight());
        relativePos += relativeWidth;
    }
}

void FavoritesBar::setBoundsOnScreen() {
    setBoundsRelative(0.0f, 0.0f, 1.0f, 0.2f);
}