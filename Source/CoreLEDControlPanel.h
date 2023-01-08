/*
  ==============================================================================

    CoreLEDControlPanel.h
    Created: 18 Dec 2022 4:47:03pm
    Author:  Tiparium

    Core LED Control features, light chosing, color picking, etc.

  ==============================================================================
*/

#pragma once

#pragma once

// Mandatory
#include <JuceHeader.h>
#include <string>

// Uses Enums
#include "../Source/PersistenceIDValues.h"

// Has
#include "../Source/FavoritesSlot.h"
#include "../Source/LEDControlCommandCodes.h"

// Has&
#include "../Source/WebServerHandler.h"
#include "../Source/PHueHandler.h"
#include "../Source/PersistenceJSONHandler.h"

class CoreLEDControlPanel :
    public juce::Component,
    public juce::Slider::Listener,
    public juce::Button::Listener
{
public:
    CoreLEDControlPanel(juce::Component* parent, 
        WebServerHandler* webServerHandler,
        PHueHandler* pHueHandler,
        PersistenceJSONHandler* persistenceJSONHandler,
        TIP_RGB* uiRGB, TIP_RGB* ledRGB);
    ~CoreLEDControlPanel() override;

    // RGBSliders
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;

    // Button Logic
    void buttonClicked(juce::Button* button) override;
    bool checkFavoritesButtons(juce::Button* button);
    bool checkLEDControlButtons(juce::Button* button);
    bool checkLEDControlButtonState(); // Returns true if all LED control buttons are active

    // Drawing Logic
    void paint(juce::Graphics&) override;
    void paintFavorites(juce::Graphics&);
    void paintSliders(juce::Graphics&);
    void paintLEDControlButtons(juce::Graphics&);
    void resized() override;


    //  G/S
    void    setSliderValues(TIP_RGB rgb);

private:

    // Refs to RGB State
    TIP_RGB* _uiRGB_Ref;
    TIP_RGB* _ledRGB_Ref;

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
    juce::TextButton            _newFavButton;

    // Individual pHue LED control
    std::vector<juce::TextButton*> _pHueLEDPickers;
    juce::TextButton               _toggleAllLEDControlButton;
    std::vector<bool>              _listeningLights;

    // Talk to the Handlers
    WebServerHandler*        _webServerHandler_Ref;
    PHueHandler*             _pHuePHueHandler_Ref;
    PersistenceJSONHandler*  _persistenceJSONHandler_Ref;
    juce::Component*         _parent;
};