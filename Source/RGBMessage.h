/*
  ==============================================================================

    RGBMessage.h
    Created: 16 Oct 2022 4:28:04pm
    Author:  Tiparium

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RGBStructs.h"

class RGBMessage : public juce::Message
{
public:
    RGBMessage::RGBMessage(RGB rgb);
    RGBMessage::~RGBMessage();

    RGB RGBMessage::getRGB();
private:
    RGB _rgb;
};