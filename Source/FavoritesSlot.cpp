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

juce::Button& FavoritesSlot::getButton() {
    return _button;
}

void FavoritesSlot::paint(juce::Graphics& g) {
    addAndMakeVisible(_button);
    _button.setBoundsRelative(0, 0, 1, 1);
}