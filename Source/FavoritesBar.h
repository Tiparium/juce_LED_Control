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

class FavoritesBar : public juce::Component, juce::ChangeListener{
public:

    FavoritesBar::FavoritesBar(RestHandler& RestHandler);
    FavoritesBar::~FavoritesBar();

    void changeListenerCallback(juce::ChangeBroadcaster* src);

    // Drawing Logic
    void paint(juce::Graphics&) override;
    // void resized() override; // Add this back if needed
    void setBoundsOnScreen();
private:

    // These are for testing stuff, delete later
    FavoritesSlot _testSlot;
    std::vector<FavoritesSlot*> _favSlots;
    juce::uint8 _tempR;
    juce::uint8 _tempG;
    juce::uint8 _tempB;

    // Talk to the handler
    RestHandler _restHandler;
};