/*
  ==============================================================================

    FavoritesBar.cpp
    Created: 13 Oct 2022 4:44:29pm
    Author:  Tiparium

  ==============================================================================
*/

#include "FavoritesBar.h"

FavoritesBar::FavoritesBar(RestHandler& restHandler):
    _restHandler(restHandler)
{

    // Literally everything below this line is temporary & only for debugging purposes
    RGB red(255, 0, 0);
    RGB green(0, 255, 0);
    RGB blue(0, 0, 255);
    RGB white(255, 255, 255);

    FavoritesSlot* slot = new FavoritesSlot();
    FavoritesSlot* slotTwo = new FavoritesSlot();
    FavoritesSlot* slotThree = new FavoritesSlot();
    FavoritesSlot* slotFour = new FavoritesSlot();

    slot->setRGB(red);
    slotTwo->setRGB(green);
    slotThree->setRGB(blue);
    slotFour->setRGB(white);


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
        if (button == &_favSlots[i]->getButton()) {
            if (!_favSlots[i]->state) {
                // Assign button values & Color
                RGB hRGB = _restHandler.getRGB();
                _favSlots[i]->setRGB(hRGB);
                _favSlots[i]->getButton().setColour(
                    juce::TextButton::ColourIds::buttonColourId , juce::Colour(hRGB._r, hRGB._g, hRGB._b));
                _favSlots[i]->state = true;
                return;
            }
            else {
                rgb = _favSlots[i]->getRGB();
                _restHandler.takeColorPushUpdate(rgb);
                return;
            }
        }
    }
    DBG("Something went wrong: This loop should never fully terminate.");
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