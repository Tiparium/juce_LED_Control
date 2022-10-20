/*
  ==============================================================================

    FavoritesSlot.h
    Created: 13 Oct 2022 4:45:04pm
    Author:  Tiparium

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Source/RGBStructs.h"

class FavoritesSlot : public juce::Component
{
public:

    FavoritesSlot::FavoritesSlot();
    FavoritesSlot::~FavoritesSlot();

    RGB getRGB();
    void setRGB(RGB rgb);

    juce::Button& getButton();

    // Drawing Logic
    void paint(juce::Graphics&) override;
    void setBoundsOnScreen();

    bool state = false;

private:
    RGB _rgb;
    juce::TextButton _button;
};