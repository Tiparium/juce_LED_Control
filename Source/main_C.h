/*
  ==============================================================================

    Main_C.h
    Created: 9 Oct 2022 12:05:31am
    Author:  Tiparium

    "The base upon which we shall build"
    - Tiparium, circa 10/9/2022 @ 12:09am
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RestHandler.h"
#include "RGBSlider.h"
#include "FavoritesBar.h"

class Main_C : public juce::Component
{
public:
    Main_C();
    ~Main_C() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    RestHandler     _restHandler;
    RGBSlider       _RGBSliders;
    FavoritesBar    _FavoritesBar;
};