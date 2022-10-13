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

#include "../Source/RestHandler_B.h"
#include "../Source/rgbStructs_D.h"

//==============================================================================

class rgbSlider_C : public juce::Component, public juce::Slider::Listener
{
public:
    rgbSlider_C(RestHandler_B& restHandler);
    ~rgbSlider_C() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void size();
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;
   
    // Put ported functions below this line
    //void getPhilipsData();
    //void grabRGBPushUpdate();
    //void pushUpdate(juce::var xyColor, juce::String target);
    //void updateRootJSON();

    //nlohmann::json pingAndReceive(juce::String target);

    //int getNumLights();

    // Debug Functions:
    void resetColor(); // Reset lights to color they were at time of app being opened

private:
    juce::String _currentSizeAsString;

    juce::Label _rLabel;
    juce::Label _gLabel;
    juce::Label _bLabel;

    juce::Slider _rSlider;
    juce::Slider _gSlider;
    juce::Slider _bSlider;

    // Comment stuff out as it's moved to the right file :)
    // juce::uint8 _rVal = 0;
    // juce::uint8 _gVal = 0;
    // juce::uint8 _bVal = 0;

    // Combine these for a full callable RESTful URL
    //juce::String _httpTarget;
    //juce::String _apiTarget;
    //juce::String _apiGetTarget;
    //juce::String _apiPutTarget;
    //int _apiPutSplit = 8; // Index of the placeholder character (X) in _apiPutTarget

    //int _numLights = -1;

    // holds response of the get all lights RESTful call
    // nlohmann::json _rootJSON;

    // For making API calls
    // adamski::RestRequest _req;
    RestHandler_B _restHandler;

    // For resetting to original color settings
    // float _OGxVal;
    // float _OGyVal;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(rgbSlider_C)
};