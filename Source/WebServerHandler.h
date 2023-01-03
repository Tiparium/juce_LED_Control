/*
  ==============================================================================

    WebServerHandler.h
    Created: 18 Nov 2022 4:03:19pm
    Author:  Tiparium

    Client side of WebServer, used for sending / receiving data from the Server
    Also needed in order to shut the server down.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../Source/RGBStructs.h"
#include "../json/include/nlohmann/json.hpp"

class WebServerHandler : public juce::Thread
{
public:

    WebServerHandler(TIP_RGB* rgbRef, std::vector<TIP_RGB>* rgbPatternRef);
    ~WebServerHandler();

    void run() override;
    void sendSingleRGBPostRequest(TIP_RGB rgb);
    void sendRGBPatternPostRequest(std::vector<TIP_RGB> pattern);

    bool checkRGBPatternEquality(std::vector<TIP_RGB> local, std::vector<TIP_RGB>* newData);

    // G/S
    void setRGBPatternRef(std::vector<TIP_RGB>* patternRef);
    // *G/S
private:

    // For making API calls
    int          _timeout = 3000;
    juce::String _addr = "http://10.0.0.104/setcolor";

    // For tracking RGB data
    TIP_RGB*                _rgbRef;
    TIP_RGB                 _localRGB = TIP_RGB(1, 2, 3);
    std::vector<TIP_RGB>*   _rgbPatternRef;
    std::vector<TIP_RGB>    _localRGBPattern;

    // For state tracking
    bool _stateOverride = false;

    // Debug
    bool    _classDebug = false;
};