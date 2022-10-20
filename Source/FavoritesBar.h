/*
  ==============================================================================

    FavoritesBar.h
    Created: 13 Oct 2022 4:44:29pm
    Author:  Tiparium

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../Source/RestHandler.h"
#include "../Source/FavoritesSlot.h"

class FavoritesBar : public juce::Component,
                     public juce::Button::Listener
{
public:

    FavoritesBar::FavoritesBar(RestHandler& RestHandler);
    FavoritesBar::~FavoritesBar();

    void buttonClicked(juce::Button* button) override;

    // Drawing Logic
    void paint(juce::Graphics&) override;
    // void resized() override; // Add this back if needed
    void setBoundsOnScreen();
private:
    std::vector<FavoritesSlot*> _favSlots;
    juce::uint8 _tempR;
    juce::uint8 _tempG;
    juce::uint8 _tempB;

    // Talk to the handler
    RestHandler _restHandler;
};