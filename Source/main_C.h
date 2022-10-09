/*
  ==============================================================================

    main_C.h
    Created: 9 Oct 2022 12:05:31am
    Author:  Tiparium

    "The base upon which we shall build"
    - Tiparium, circa 10/9/2022 @ 12:09am
  ==============================================================================
*/

#include <JuceHeader.h>
#include "rgbSlider_C.h"

class Main_C : public juce::Component
{
public:
    Main_C();
    ~Main_C() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    rgbSlider_C _rgbSliders;
};

#pragma once
