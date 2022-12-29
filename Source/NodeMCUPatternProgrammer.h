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

class NodeMCUPatternProgrammer :
    public juce::Component,
    public juce::Slider::Listener,
    public juce::Button::Listener
{
public:

    NodeMCUPatternProgrammer(juce::Component* parent, WebServerHandler* webServerHandler, PHueHandler* pHueHandler, PersistenceJSONHandler* persistenceJSONHandler, TIP_RGB* uiRGB, TIP_RGB* ledRGB);
    ~NodeMCUPatternProgrammer();

    // RGBSliders
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;

    // Button Logic
    void buttonClicked(juce::Button* button) override;

    // G/S
    // *G/S

private:

    // Refs to RGB State
    TIP_RGB* _uiRGB_Ref;
    TIP_RGB* _ledRGB_Ref;

    // Talk to the Handlers
    // Talk to the Handlers
    WebServerHandler*           _webServerHandler_Ref;
    PHueHandler*                _pHuePHueHandler_Ref;
    PersistenceJSONHandler*     _persistenceJSONHandler_Ref;
    juce::Component*            _parent;
};