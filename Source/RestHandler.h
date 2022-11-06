/*
  ==============================================================================

    RestHandler.h
    Created: 12 Oct 2022 9:20:59pm
    Author:  Tiparium

    This class is responsible for communicating with the pHue RestAPI
    Data is also stored representing the current overall colorstate of the program

  ==============================================================================
*/

#pragma once

#include "../Source/RGBStructs.h"
#include "../RestRequest/rest_request.h"
#include "../json/include/nlohmann/json.hpp"
#include "../resources/params.h" // Replace with custom params file

class RestHandler{
public:
    RestHandler(juce::String http, juce::String api, juce::String get, juce::String put);
    ~RestHandler();

    void takeColorPushUpdate(RGB rgb);
    void grabColorPushUpdate();

    void pushRGBToPHue(RGB rgb, juce::String target);
    void pushXYBToPHue(juce::var xyColor, juce::String target);

    void updateRootJSON();
    
    //  G/S
    void setRGB(RGB rgb);
    RGB getRGB();
    void setR(juce::uint8 val);
    void setG(juce::uint8 val);
    void setB(juce::uint8 val);
    juce::uint8 getR();
    juce::uint8 getG();
    juce::uint8 getB();
    int getNumLights();
    //* G/S
    //  Debug
    void resetColor(); // Reset lights to color they were at time of app being opened
    void printXY();
    //* Debug
private:

    // Represents the current RGB state of all lights
    juce::uint8 _rVal;
    juce::uint8 _gVal;
    juce::uint8 _bVal;

    // Combine these for a full callable RESTful URL
    juce::String _httpTarget;
    juce::String _apiTarget;
    juce::String _apiGetTarget;
    juce::String _apiPutTarget;
    int _apiPutSplit = 8; // Index of the placeholder character (X) in _apiPutTarget

    // holds response of the get all lights RESTful call
    nlohmann::json _rootJSON;

    // Derived from _rootJSON
    int _numLights;

    // For making API calls
    adamski::RestRequest _req;

    // For resetting to original color settings
    float _OGxVal;
    float _OGyVal;
};