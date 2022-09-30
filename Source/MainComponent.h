/*
  ==============================================================================

    MainComponent.h
    Created: 26 Sep 2022 11:09:01am
    Author:  Tiparium
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <fstream>
#include <iostream>  
#include <string>  
#include "rest_request.h"
#include "json.h"
#include <nlohmann/json.hpp>
#include "Vars.h"

//==============================================================================
/*
*/
class MainComponent : public juce::Component, public juce::Slider::Listener
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void size();
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;
    void getPhilipsData();
    void pushPhilipsDataUpdate();

private:
    juce::String _currentSizeAsString;

    const juce::String _httpTarget = "http://10.0.0.139";

    const juce::String _apiTarget = DataVault::getAPITarget();
    
    const juce::String _apiGetTarget = "/lights";

    // Change character 7(x) to assign to a specific light
    juce::String _apiPutTarget = "lights/x/state";

    juce::Label _rLabel;
    juce::Label _gLabel;
    juce::Label _bLabel;

    juce::Slider _rSlider;
    juce::Slider _gSlider;
    juce::Slider _bSlider;

    juce::uint8 _rVal = 0;
    juce::uint8 _gVal = 0;
    juce::uint8 _bVal = 0;

    adamski::RestRequest _req;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
