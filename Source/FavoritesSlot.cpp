/*
  ==============================================================================

    FavoritesSlot.cpp
    Created: 13 Oct 2022 4:45:04pm
    Author:  Tiparium

  ==============================================================================
*/

#include "FavoritesSlot.h"

FavoritesSlot::FavoritesSlot()
{
    _button.addListener(this);
}

FavoritesSlot::~FavoritesSlot() {
    _button.removeListener(this);
}

void FavoritesSlot::buttonClicked(juce::Button* button) {
    bool dbg = true;

    if (dbg) {
        juce::String rBit = "Slot R: " + juce::String(_R);
        juce::String gBit = "Slot G: " + juce::String(_G);
        juce::String bBit = "Slot B: " + juce::String(_B);
        juce::String debugStr = rBit + "\n" + gBit + "\n" + bBit + "\n";
        DBG(debugStr);
    }
    RGB rgb = grabCurrentRGB();
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
juce::Button& FavoritesSlot::getButton() {
    return _button;
}


RGB FavoritesSlot::grabCurrentRGB() {
    RGB rgb;
    rgb.r = _R;
    rgb.g = _G;
    rgb.b = _B;
    return rgb;
}

void FavoritesSlot::paint(juce::Graphics& g) {
    _button.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
    addAndMakeVisible(_button);
}

void FavoritesSlot::setBoundsOnScreen() {
    setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
}