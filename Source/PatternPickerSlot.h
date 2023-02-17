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
#include "../Source/LEDControlCommandCodes.h"

class PatternPickerSlot : public juce::Component
{
public:
    PatternPickerSlot(juce::Component* parent, TIP_RGB rgb);
    ~PatternPickerSlot();
    void initButtonLogic();
    // RGB Data
    TIP_RGB getTrueRGB();
    TIP_RGB getCCRGB();
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

    enum {
        _slot_B_ID,
        _del_B_ID,
        _toggleMode_B_ID,
        _activityIndicator_ID
    };

private:
    // UI Elements
    juce::TextButton _slot_B;            //  0
    juce::TextButton _del_B;             //  1
    juce::TextButton _toggleMode_B;      //  2
    juce::TextButton _activityIndicator; // -1

    // Data
    bool             _isActiveSlot;
    TIP_RGB          _localRGB;
    bool             _useColorCorrect = true;

    // Intercomms
    juce::Component* _parent;

    // DEBUG
    int _repaintCounter = 0;
};