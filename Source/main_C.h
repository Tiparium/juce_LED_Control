/*
  ==============================================================================

    Main_C.h
    Created: 9 Oct 2022 12:05:31am
    Author:  Tiparium

    "The base upon which we shall build"
    - Tiparium, circa 10/9/2022 @ 12:09am

    Contains & Displays subcomponents both for data processing & UI display.

    // TODO - Save the state of what lights are active, don't just init everything to true
    // TODO - Wrap all handlers in their own threads so the program doesn't hang for a million years
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <string>

#include "../Source/CoreLEDControlPanel.h"
#include "../Source/NodeMCUPatternProgrammer.h"
#include "../Source/LEDControlCommandCodes.h"
#include "../Source/WebServerHandler.h"
#include "../Source/PHueHandler.h"
#include "../Source/PersistenceJSONHandler.h"

class Main_C : public juce::Component
{
public:
    Main_C();
    ~Main_C() override;

    void handleCommandMessage(int commandId) override;
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // Tab Pannel Selector
    juce::TabbedComponent* _panelSelector;

    // DisplayTabs
    CoreLEDControlPanel*        _coreLEDControlPanel;
    NodeMCUPatternProgrammer*   _nodeMCUPatternProgrammer;

    // Current RGB state
    TIP_RGB                 _uiRGB;      // Represents RGB state displayed on screen
    TIP_RGB                 _ledRGB;     // Represents RGB state displayed by LEDS
    std::vector<TIP_RGB>    _rgbPattern;

    // Talk To The Handlers -> How I send data to places
    WebServerHandler        _webServerHandler;
    PHueHandler             _pHuePHueHandler;
    PersistenceJSONHandler  _persistenceJSONHandler;
};