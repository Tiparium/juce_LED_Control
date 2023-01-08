/*
  ==============================================================================

    PatternPickerSlot.cpp
    Created: 7 Jan 2023 12:11:44pm
    Author:  Tiparium

  ==============================================================================
*/

#include "PatternPickerSlot.h"


PatternPickerSlot::PatternPickerSlot(TIP_RGB rgb) :
    _isActiveSlot(true)
{
    _ledRGB.r = rgb.r;
    _ledRGB.g = rgb.g;
    _ledRGB.b = rgb.b;
    _delButton.setButtonText("X");
    _toggleModeButton.setButtonText("C");
    _toggleModeButton.setHelpText("Toggle Pattern Node between Color Corrected & Absolute mode");
    setSlotButtonColors();
}
PatternPickerSlot::~PatternPickerSlot() {}

TIP_RGB PatternPickerSlot::getRGB() {
    if (_useColorCorrect) { return _ledRGB.colorCorrect(); }
    return _ledRGB;
}

void PatternPickerSlot::setRGB(TIP_RGB rgb) {
    _ledRGB = rgb;
    setSlotButtonColors();
}

juce::Button& PatternPickerSlot::getButton(int button) {
    if (button == 0) return _slotButton;
    if (button == 1) return _delButton;
    if (button == 2) return _toggleModeButton;
    DBG("A request has been made for a button which does not exist with in a Pattern Picker Slot");
}

void PatternPickerSlot::toggleMode()
{
    _useColorCorrect = !_useColorCorrect;
    repaint();
}

bool PatternPickerSlot::getMode()
{
    return _useColorCorrect;
}

// TODO: Send an update message to the NodeMCUPatternProgrammer when this is called
void PatternPickerSlot::setActive()
{
    _isActiveSlot = true;
    repaint();
}

// TODO: Send an update message to the NodeMCUPatternProgrammer when this is called
void PatternPickerSlot::setInactive()
{
    _isActiveSlot = false;
    repaint();
}

void PatternPickerSlot::paint(juce::Graphics& g) {
    addAndMakeVisible(_slotButton);
    _slotButton.setBoundsRelative(0, 0, 1, 1);
    addAndMakeVisible(_delButton);
    _delButton.setBoundsRelative(0.75f, 0.0f, 0.25f, 0.25f);
    addAndMakeVisible(_toggleModeButton);
    _toggleModeButton.setBoundsRelative(0.75f, 0.75f, 0.25f, 0.25f);
    addAndMakeVisible(_activityIndicator);
    _activityIndicator.setBoundsRelative(0.0f, 0.0f, 0.25f, 1.0f);

    setSlotButtonColors();

    juce::String toggleMode = _useColorCorrect ? "C" : "A";
    _toggleModeButton.setButtonText(toggleMode);
}

void PatternPickerSlot::setSlotButtonColors()
{
    TIP_RGB rgbToAssign;
    if (_useColorCorrect) { rgbToAssign = TIP_RGB(_ledRGB.colorCorrect()); }
    else { rgbToAssign = TIP_RGB(_ledRGB); }

    _slotButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(rgbToAssign.r, rgbToAssign.g, rgbToAssign.b));

    if (_isActiveSlot)
    {
        _activityIndicator.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
    }
    else
    {
        _activityIndicator.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(rgbToAssign.r, rgbToAssign.g, rgbToAssign.b));
    }
}