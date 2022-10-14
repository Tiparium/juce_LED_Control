/*
  ==============================================================================

    FavoritesBar.cpp
    Created: 13 Oct 2022 4:44:29pm
    Author:  Tiparium

  ==============================================================================
*/

#include "FavoritesBar.h"

FavoritesBar::FavoritesBar(RestHandler& RestHandler) {
    _restHandler = RestHandler;
}

FavoritesBar::~FavoritesBar() {

}

//Mandatory override
void FavoritesBar::buttonClicked(juce::Button* button) {

}