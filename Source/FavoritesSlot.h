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

    FavoritesSlot::FavoritesSlot(RGB rgb);
    FavoritesSlot::~FavoritesSlot();

    RGB getRGB();
    void setRGB(RGB rgb);

    juce::Button& getButton(int button);

    // Drawing Logic
    void paint(juce::Graphics&) override;

private:
    RGB _rgb;
    juce::TextButton _favButton;
    juce::TextButton _delButton;
};