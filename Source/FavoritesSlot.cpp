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
    _del_B.setButtonText("X");
    _fav_B.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(_ledRGB.r, _ledRGB.g, _ledRGB.b));
}
FavoritesSlot::~FavoritesSlot(){}

TIP_RGB FavoritesSlot::getCCRGB() {
    return _ledRGB;
}

void FavoritesSlot::setRGB(TIP_RGB rgb) {
    _ledRGB = rgb;
}

juce::Button& FavoritesSlot::getButton(int button) {
    if (button == _fav_B_ID) return _fav_B;
    if (button == _del_B_ID) return _del_B;
    DBG("You shouldn't be here");
}

void FavoritesSlot::paint(juce::Graphics& g) {
    addAndMakeVisible(_fav_B);
    _fav_B.setBoundsRelative(0, 0, 1, 1);
    addAndMakeVisible(_del_B);
    _del_B.setBoundsRelative(0.75f, 0.0f, 0.25f, 0.25f);
}