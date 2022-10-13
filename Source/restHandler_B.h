/*
  ==============================================================================

    RestHandler_B.h
    Created: 12 Oct 2022 9:20:59pm
    Author:  Tiparium

  ==============================================================================
*/

#pragma once

#include "../Source/rgbStructs_D.h"
#include "../RestRequest/rest_request.h"
#include "../json/include/nlohmann/json.hpp"
#include "../resources/params.h" // Replace with custom params file

class RestHandler_B{
public:
    RestHandler_B();
    ~RestHandler_B();

    void getPhilipsData();
    void grabRGBPushUpdate();
    void pushUpdate(juce::var xyColor, juce::String target);
    void updateRootJSON();
    nlohmann::json pingAndReceive(juce::String target);
    int getNumLights();
    void resetColor(); // Reset lights to color they were at time of app being opened
    // G/S
    void setR(juce::uint8 val);
    void setG(juce::uint8 val);
    void setB(juce::uint8 val);
    juce::uint8 getR();
    juce::uint8 getG();
    juce::uint8 getB();
    //* G/S
private:

    // Represents the current RGB state of all lights
    juce::uint8 _rVal = 0;
    juce::uint8 _gVal = 0;
    juce::uint8 _bVal = 0;

    // Combine these for a full callable RESTful URL
    juce::String _httpTarget;
    juce::String _apiTarget;
    juce::String _apiGetTarget;
    juce::String _apiPutTarget;
    int _apiPutSplit = 8; // Index of the placeholder character (X) in _apiPutTarget

    // holds response of the get all lights RESTful call
    nlohmann::json _rootJSON;

    // Derived from _rootJSON
    int _numLights = -1;

    // For making API calls
    adamski::RestRequest _req;

    // For resetting to original color settings
    float _OGxVal;
    float _OGyVal;
};