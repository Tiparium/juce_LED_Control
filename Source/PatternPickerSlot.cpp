/*
  ==============================================================================

    PatternPickerSlot.cpp
    Created: 7 Jan 2023 12:11:44pm
    Author:  Tiparium

  ==============================================================================
*/

#include "PatternPickerSlot.h"


PatternPickerSlot::PatternPickerSlot(juce::Component* parent, TIP_RGB rgb) :
    _parent(parent),
    _isActiveSlot(true)
{
    _localRGB.r = rgb.r;
    _localRGB.g = rgb.g;
    _localRGB.b = rgb.b;
    _del_B.setButtonText("X");
    _toggleMode_B.setButtonText("C");
    _toggleMode_B.setHelpText("Toggle Pattern Node between Color Corrected & Absolute mode");
    _parent->postCommandMessage(pattern_picker_mode_updated);

    initButtonLogic();
}
PatternPickerSlot::~PatternPickerSlot() {}

void PatternPickerSlot::initButtonLogic()
{
    _toggleMode_B.onClick = [this] { toggleMode(); };
}

// Returns non color corrected RGB
TIP_RGB PatternPickerSlot::getTrueRGB()
{
    return _localRGB;
}

// Returns color corrected RGB
TIP_RGB PatternPickerSlot::getRGB() {
    if (_useColorCorrect) { return _localRGB.colorCorrect(); }
    return _localRGB;
}

void PatternPickerSlot::setRGB(TIP_RGB rgb) {
    _localRGB = rgb;
    setSlotButtonColors();
}

juce::Button& PatternPickerSlot::getButton(int button) {
    if (button == _activityIndicator_ID) return _activityIndicator; // -1
    if (button == _slot_B_ID) return _slot_B;                       //  0
    if (button == _del_B_ID) return _del_B;                         //  1
    if (button == _toggleMode_B_ID) return _toggleMode_B;           //  2
    DBG("A request has been made for a button which does not exist with in a Pattern Picker Slot");
}

void PatternPickerSlot::toggleMode()
{
    _useColorCorrect = !_useColorCorrect;
    if (_isActiveSlot)
    {
        _parent->postCommandMessage(pattern_picker_mode_updated);
    }
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
    addAndMakeVisible(_slot_B);
    _slot_B.setBoundsRelative(0, 0, 1, 1);
    addAndMakeVisible(_del_B);
    _del_B.setBoundsRelative(0.75f, 0.0f, 0.25f, 0.25f);
    addAndMakeVisible(_toggleMode_B);
    _toggleMode_B.setBoundsRelative(0.75f, 0.75f, 0.25f, 0.25f);
    addAndMakeVisible(_activityIndicator);
    _activityIndicator.setBoundsRelative(0.0f, 0.0f, 0.25f, 1.0f);

    setSlotButtonColors();

    juce::String buttonMode = _useColorCorrect ? "C" : "A";
    _toggleMode_B.setButtonText(buttonMode);
}

void PatternPickerSlot::setSlotButtonColors()
{
    TIP_RGB rgbToAssign;
    if (_useColorCorrect) { rgbToAssign = TIP_RGB(_localRGB.colorCorrect()); }
    else { rgbToAssign = TIP_RGB(_localRGB); }

    _slot_B.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(rgbToAssign.r, rgbToAssign.g, rgbToAssign.b));

    if (_isActiveSlot)
    {
        _activityIndicator.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
    }
    else
    {
        _activityIndicator.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(rgbToAssign.r, rgbToAssign.g, rgbToAssign.b));
    }
}