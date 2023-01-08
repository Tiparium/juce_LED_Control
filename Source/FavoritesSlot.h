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

    FavoritesSlot(TIP_RGB rgb);
    ~FavoritesSlot();

    TIP_RGB getRGB();
    void setRGB(TIP_RGB rgb);

    juce::Button& getButton(int button);

    // Drawing Logic
    void paint(juce::Graphics&) override;

private:
    TIP_RGB          _ledRGB;
    juce::TextButton _favButton;
    juce::TextButton _delButton;
};