/*
  ==============================================================================
    NOTES: None :)

    rgbSlider_C.h
    Created: 8 Oct 2022 11:44:25pm
    Author:  Tiparium

    Creates & controls the 3 RGB sliders on main screen
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

private:
    juce::String _currentSizeAsString;

    juce::Label _rLabel;
    juce::Label _gLabel;
    juce::Label _bLabel;

    juce::Slider _rSlider;
    juce::Slider _gSlider;
    juce::Slider _bSlider;

    // Talk to the handler
    RestHandler_B _restHandler;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(rgbSlider_C)
};