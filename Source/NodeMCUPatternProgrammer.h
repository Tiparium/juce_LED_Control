/*
  ==============================================================================

    NodeMCUPatternProgrammer.h
    Created: 26 Dec 2022 5:23:42pm
    Author:  Tiparium

    Allows uploading of specific patterns to the NodeMCU,
        as well as toggling other features.

    Makes changes to NodeMCU via reference to CoreLEDControlPanel's handler.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

// Has&
#include "../Source/WebServerHandler.h"
#include "../Source/PHueHandler.h"
#include "../Source/PersistenceJSONHandler.h"
#include "../Source/PatternPickerSlot.h"

class NodeMCUPatternProgrammer :
    public juce::Component,
    public juce::Slider::Listener,
    public juce::Button::Listener
{
public:

    NodeMCUPatternProgrammer(juce::Component* parent, WebServerHandler* webServerHandler, PHueHandler* pHueHandler, PersistenceJSONHandler* persistenceJSONHandler, TIP_RGB* uiRGB, TIP_RGB* ledRGB);
    ~NodeMCUPatternProgrammer();

    void handleCommandMessage(int commandId) override;

    // Painting Logic
    void paint(juce::Graphics& g) override;
    void paintSliders(juce::Graphics& g);
    void paintPatternGenerator(juce::Graphics& g);

    // RGBSliders
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;
    TIP_RGB buildRGBFromSliders();

    // Button Logic
    void buttonClicked(juce::Button* button) override;
    bool checkPatternButtons(juce::Button* button);
    bool checkMultiplierButtons(juce::Button* button);
    bool checkScrollingButtons(juce::Button* button);
    bool checkSaveLoadUploadButtons(juce::Button* button);

    void setActiveSlot(int slotIndex);

    //  G/S
    void setSliderValues(TIP_RGB rgb);
    // *G/S

private:
    // Refs to RGB State
    TIP_RGB* _uiRGB_Ref;
    TIP_RGB* _ledRGB_Ref;

    // Local RGB Background State
    TIP_RGB  _localRGB = TIP_RGB(255, 255, 255);

    // RGBSliders
    juce::Label _rLabel;
    juce::Label _gLabel;
    juce::Label _bLabel;
    juce::Slider _rSlider;
    juce::Slider _gSlider;
    juce::Slider _bSlider;
    //* RGBSliders

    // Pattern picker bar
    std::vector<PatternPickerSlot*> _patternPickers;
    PatternPickerSlot*              _currentSlot;
    juce::TextButton                _newNodeButton;

    // Save / Load / Upload
    juce::TextButton                _UploadPattern;

    // Talk to the Handlers
    WebServerHandler*           _webServerHandler_Ref;
    PHueHandler*                _pHuePHueHandler_Ref;
    PersistenceJSONHandler*     _persistenceJSONHandler_Ref;
    juce::Component*            _parent;
};