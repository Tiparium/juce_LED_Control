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
#include <string>

#include "../Source/PersistenceJSONHandler.h"
#include "../Source/RestHandler.h"
#include "../Source/FavoritesSlot.h"
#include "../Source/WebServerHandler.h"

class Main_C : public juce::Component,
               public juce::Slider::Listener,
               public juce::Button::Listener
{
public:
    Main_C();
    ~Main_C() override;

    // RGBSliders
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;

    // FavoritesBar
    void buttonClicked(juce::Button* button) override;

    // Drawing Logic
    void paint(juce::Graphics&) override;
    void paintFavorites(juce::Graphics&);
    void paintSliders(juce::Graphics&);
    void paintLEDSelector(juce::Graphics&);
    void resized() override;
    //  G/S
    void setSliderValues(TIP_RGB rgb);
private:

    // RGBSliders
    juce::Label _rLabel;
    juce::Label _gLabel;
    juce::Label _bLabel;
    juce::Slider _rSlider;
    juce::Slider _gSlider;
    juce::Slider _bSlider;
    //* RGBSliders

    // FavoritesBar
    std::vector<FavoritesSlot*> _favSlots;
    juce::TextButton _newFavButton;

    // Individual pHue LED control
    std::vector<juce::TextButton*> _pHueLEDPickers;
    juce::TextButton               _allPHueLEDSButton;
    int*                           _pHueLEDCount;

    // Talk to the Handlers
    WebServerHandler        _nodeMCUServerHandler;
    RestHandler             _pHueRestHandler;
    PersistenceJSONHandler  _favsHandler;
};