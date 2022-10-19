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

class FavoritesSlot : public juce::Component,
                      public juce::ChangeBroadcaster, // May be DRPCD
                      //public juce::MessageListener,
                      public juce::Button::Listener
{
// GUIDE (DELETE THIS)
    // A = FavoritesBar
    // Z = FavoritesSlot
public:

    FavoritesSlot::FavoritesSlot();
    FavoritesSlot::~FavoritesSlot();

    void buttonClicked(juce::Button* button) override;

    juce::uint8 getR();
    juce::uint8 getG();
    juce::uint8 getB();
    juce::Button& getButton();

    RGB grabCurrentRGB();

    // Drawing Logic
    void paint(juce::Graphics&) override;
    // void resized() override; // Add this back if needed
    void setBoundsOnScreen();

private:
    juce::uint8 _R = 55;
    juce::uint8 _G = 55;
    juce::uint8 _B = 55;

    juce::TextButton _button = juce::TextButton("Hello World!");
};