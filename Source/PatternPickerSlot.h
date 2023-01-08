/*
  ==============================================================================

    PatternPickerSlot.h
    Created: 7 Jan 2023 12:11:44pm
    Author:  Tiparium

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Source/RGBStructs.h"

class PatternPickerSlot : public juce::Component
{
public:
    PatternPickerSlot(TIP_RGB rgb);
    ~PatternPickerSlot();

    // RGB Data
    TIP_RGB getRGB();
    void setRGB(TIP_RGB rgb);

    // Mode and activity
    juce::Button& getButton(int button);
    void toggleMode();
    bool getMode();
    void setActive();
    void setInactive();

    // Drawing Logic
    void paint(juce::Graphics&) override;
    void setSlotButtonColors();


private:
    // UI Elements
    juce::TextButton _slotButton;
    juce::TextButton _delButton;
    juce::TextButton _toggleModeButton;
    juce::TextButton _activityIndicator;

    // Data
    bool             _isActiveSlot;
    TIP_RGB          _ledRGB;
    bool             _useColorCorrect = true;
};