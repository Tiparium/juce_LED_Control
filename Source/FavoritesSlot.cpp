/*
  ==============================================================================

    FavoritesSlot.cpp
    Created: 13 Oct 2022 4:45:04pm
    Author:  Tiparium

  ==============================================================================
*/

#include "FavoritesSlot.h"

FavoritesSlot::FavoritesSlot(TIP_RGB rgb)
{
    _ledRGB.r = rgb.r;
    _ledRGB.g = rgb.g;
    _ledRGB.b = rgb.b;
    _delButton.setButtonText("X");
    _favButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(_ledRGB.r, _ledRGB.g, _ledRGB.b));
}
FavoritesSlot::~FavoritesSlot(){}

TIP_RGB FavoritesSlot::getRGB() {
    return _ledRGB;
}

void FavoritesSlot::setRGB(TIP_RGB rgb) {
    _ledRGB = rgb;
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