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
    _delButton.setButtonText("X");
    _rgb.r = rgb.r;
    _rgb.g = rgb.g;
    _rgb.b = rgb.b;
    _favButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(_rgb.r, _rgb.g, _rgb.b));
}
FavoritesSlot::~FavoritesSlot(){}

TIP_RGB FavoritesSlot::getRGB() {
    return _rgb;
}
void FavoritesSlot::setRGB(TIP_RGB rgb) {
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