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

#include <JuceHeader.h>
#include <string>

#include "../Source/PersistenceJSONHandler.h"
#include "../Source/PHueHandler.h"
#include "../Source/FavoritesSlot.h"
#include "../Source/WebServerHandler.h"
#include "../Source/LEDControlCommandCodes.h"

class CoreLEDControlPanel :
    public juce::Component,
    public juce::Slider::Listener,
    public juce::Button::Listener
{
public:
    CoreLEDControlPanel(juce::Component* parent);
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
    TIP_RGB getRGB();
    TIP_RGB getTempRGB();
private:

    // RGBSliders
    juce::Label _rLabel;
    juce::Label _gLabel;
    juce::Label _bLabel;
    juce::Slider _rSlider;
    juce::Slider _gSlider;
    juce::Slider _bSlider;
    //* RGBSliders

    // Current RGB state
    TIP_RGB _uiRGB; // Represents RGB state displayed on screen
    TIP_RGB _ledRGB;     // Represents RGB state displayed by LEDS

    // FavoritesBar
    std::vector<FavoritesSlot*> _favSlots;
    juce::TextButton            _newFavButton;

    // Individual pHue LED control
    std::vector<juce::TextButton*> _pHueLEDPickers;
    juce::TextButton               _toggleAllLEDControlButton;
    std::vector<bool>              _listeningLights;

    // Talk to the Handlers
    WebServerHandler        _nodeMCUServerHandler;
    PHueHandler             _pHuePHueHandler;
    PersistenceJSONHandler  _favsHandler;
    juce::Component*        _parent;
};