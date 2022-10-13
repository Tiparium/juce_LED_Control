/*
  ==============================================================================
    NOTE:
        BTODO:
            Currently all data pushing happens through this file.
            This should ONLY be the rgb sliders. PHue data pushes should happen elsewhere.
            Handle it.
            TODO: Break into _C & _B files
    rgbSlider_C.h
    Created: 8 Oct 2022 11:44:25pm
    Author:  Tiparium

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <fstream>
#include <iostream>  
#include <string>  
#include "../RestRequest/rest_request.h"
#include "../json/include/nlohmann/json.hpp"
#include "../resources/params.h" // Replace with custom params file

#include "../Source/rgbStructs_D.h"

//==============================================================================

class rgbSlider_C : public juce::Component, public juce::Slider::Listener
{
public:
    rgbSlider_C();
    ~rgbSlider_C() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void size();
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;
    void getPhilipsData();
    void grabRGBPushUpdate();
    void pushUpdate(juce::var xyColor, juce::String target);
    void updateRootJSON();

    nlohmann::json pingAndReceive(juce::String target);

    int getNumLights();

    // Debug Functions:
    void resetColor(); // Reset lights to color they were at time of app being opened

private:
    juce::String _currentSizeAsString;

    // Combine these for a full callable RESTful URL
    juce::String _httpTarget;
    juce::String _apiTarget;
    juce::String _apiGetTarget;
    juce::String _apiPutTarget;
    int _apiPutSplit = 8; // Index of the placeholder character (X) in _apiPutTarget

    juce::Label _rLabel;
    juce::Label _gLabel;
    juce::Label _bLabel;

    juce::Slider _rSlider;
    juce::Slider _gSlider;
    juce::Slider _bSlider;

    juce::TextButton _testBoi{ "Hello World!" };

    juce::uint8 _rVal = 0;
    juce::uint8 _gVal = 0;
    juce::uint8 _bVal = 0;

    int _numLights = -1;

    // holds response of the get all lights RESTful call
    nlohmann::json _rootJSON;

    // For making API calls
    adamski::RestRequest _req;

    // For resetting to original color settings
    float _OGxVal;
    float _OGyVal;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(rgbSlider_C)
};