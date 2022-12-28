/*
  ==============================================================================

    WebServerHandler.h
    Created: 18 Nov 2022 4:03:19pm
    Author:  Tiparium

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../Source/RGBStructs.h"
#include "../json/include/nlohmann/json.hpp"

class WebServerHandler : juce::Thread
{
public:

    WebServerHandler(TIP_RGB* rgbRef);
    ~WebServerHandler();

    void run() override;
    void sendSingleRGBPostRequest(TIP_RGB rgb);
private:

    // For making API calls
    int          _timeout = 5000;
    juce::String _addr = "http://10.0.0.104/setcolor";

    // For tracking RGB data
    TIP_RGB*     _rgbRef;
    TIP_RGB      _localRGB = TIP_RGB(255, 255, 255);

    // Debug
    bool    _classDebug = false;
};