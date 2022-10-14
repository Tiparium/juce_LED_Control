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

class FavoritesBar :
    public juce::Component,
    public juce::Button::Listener { // TODO: Make sure this is needed
public:

    FavoritesBar::FavoritesBar(RestHandler& RestHandler);
    FavoritesBar::~FavoritesBar();

    void buttonClicked(juce::Button* button) override;

private:


    // Talk to the handler
    RestHandler _restHandler;
};