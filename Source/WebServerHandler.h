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
#include "../Source/RGBStructs.h"

class WebServerHandler : public juce::Thread
{
public:

    WebServerHandler();
    ~WebServerHandler();



    void pushToServer(TIP_RGB rgb);
    void pushToServer_NativeJUCE(TIP_RGB rgb);

private:
    // restincurl::Client      _client;

    // For making API calls
    int                     _timeout = 5000;
    juce::String            _addr = "http://10.0.0.104/setcolor";
};