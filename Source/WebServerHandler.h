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

    WebServerHandler(juce::String addr);
    ~WebServerHandler();

    void pushToServer(TIP_RGB rgb);

private:

    // For making API calls
    adamski::RestRequest    _req;
    juce::String            _addr;
};