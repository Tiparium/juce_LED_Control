/*
  ==============================================================================

    FavoritesSlot.cpp
    Created: 13 Oct 2022 4:45:04pm
    Author:  Tiparium

  ==============================================================================
*/

#include "FavoritesSlot.h"
FavoritesSlot::FavoritesSlot() {
    _rgb._r = 0;
    _rgb._g = 0;
    _rgb._b = 0;
}

FavoritesSlot::FavoritesSlot(RGB rgb)
{
    _rgb._r = rgb._r;
    _rgb._g = rgb._g;
    _rgb._b = rgb._b;
}
FavoritesSlot::~FavoritesSlot(){}

RGB FavoritesSlot::getRGB() {
    return _rgb;
}
void FavoritesSlot::setRGB(RGB rgb) {
    _rgb = rgb;
}

juce::Button& FavoritesSlot::getButton(int button) {
    if (button == 0) return _favButton;
    if (button == 1) return _delButton;
    DBG("You shouldn't be here");
}

void FavoritesSlot::paint(juce::Graphics& g) {
    addAndMakeVisible(_favButton);
    _favButton.setBoundsRelative(0, 0, 1, 1);
    addAndMakeVisible(_delButton);
    _delButton.setBoundsRelative(0.75f, 0.0f, 0.25f, 0.25f);
}