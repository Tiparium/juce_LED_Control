/*
  ==============================================================================

    FavoritesSlot.cpp
    Created: 13 Oct 2022 4:45:04pm
    Author:  Tiparium

  ==============================================================================
*/

#include "FavoritesSlot.h"

FavoritesSlot::FavoritesSlot() {
}

FavoritesSlot::~FavoritesSlot() {
}

void FavoritesSlot::buttonClicked(juce::Button* button) {

}

juce::uint8 FavoritesSlot::getR() {
    return _R;
}
juce::uint8 FavoritesSlot::getG() {
    return _G;
}
juce::uint8 FavoritesSlot::getB() {
    return _B;
}

void FavoritesSlot::grabCurrentRGB() {

}

void FavoritesSlot::paint(juce::Graphics& g) {
    _button.setBoundsRelative(0.0f, 0.0f, 0.5f, 1.0f);
    addAndMakeVisible(_button);
}

void FavoritesSlot::setBoundsOnScreen() {
    setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
}