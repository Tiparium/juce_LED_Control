/*
  ==============================================================================

    WebServerHandler.h
    Created: 18 Nov 2022 4:03:19pm
    Author:  Tiparium

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../RestRequest/rest_request.h"
#include "../Source/RGBStructs.h"
#include "../json/include/nlohmann/json.hpp"
#include "../Source/RGBStructs.h"

class WebServerHandler
{
public:

    WebServerHandler();
    ~WebServerHandler();

    void pushToServer(TIP_RGB rgb);

private:

    // For making API calls
    adamski::RestRequest    _req;
    juce::String            _addr = "http://10.0.0.104/setcolor";
};