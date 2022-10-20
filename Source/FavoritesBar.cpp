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

    // Literally everything below this line is temporary & only for debugging purposes
    RGB zeros;
    zeros.setVals(0, 0, 0);
    RGB twentyFives;
    twentyFives.setVals(25, 25, 25);
    RGB fifties;
    fifties.setVals(50, 50, 50);
    RGB eightyTwos;
    eightyTwos.setVals(82, 82, 82);

    FavoritesSlot* slot = new FavoritesSlot();
    FavoritesSlot* slotTwo = new FavoritesSlot();
    FavoritesSlot* slotThree = new FavoritesSlot();
    FavoritesSlot* slotFour = new FavoritesSlot();

    slot->setRGB(zeros);
    slotTwo->setRGB(twentyFives);
    slotThree->setRGB(fifties);
    slotFour->setRGB(eightyTwos);


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
    juce::String ith;
    juce::String message;
    for (unsigned int i = 0; i < _favSlots.size(); i++) {
        if (button == &_favSlots[i]->getButton()) {
            rgb = _favSlots[i]->getRGB();
            ith = std::to_string(i);
            message = "th Button Has Been Clicked!";
            break;
        }
        else {
            ith = "";
            message = "Something Went Wrong :(";
        }
    }
    juce::String debug = ith + message + rgb.toString();
    DBG(debug);
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