/*
  ==============================================================================
    NOTES: None :)

    RGBSlider.h
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

#include "../Source/RestHandler.h"

//==============================================================================

class RGBSlider : public juce::Component, public juce::Slider::Listener
{
public:
    RGBSlider(RestHandler& restHandler);
    ~RGBSlider() override;

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
    RestHandler _restHandler;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RGBSlider)
};