/*
  ==============================================================================

    RGBSlider.cpp
    Created: 8 Oct 2022 11:44:25pm
    Author:  Tiparium

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RGBSlider.h"

//==============================================================================
RGBSlider::RGBSlider(RestHandler& restHandler)
{
    _restHandler = restHandler;
}

RGBSlider::~RGBSlider()
{
}

void RGBSlider::paint(juce::Graphics& g)
{
    setSize(600, 400);
    size();
    g.drawRect(getLocalBounds(), 1);
    g.fillAll(juce::Colour(_restHandler.getR(), _restHandler.getG(), _restHandler.getB())); // Set the color of the window to be the current RGB value

    g.setColour(juce::Colours::grey);

    // Red Slider
    addAndMakeVisible(_rSlider);
    _rSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    _rSlider.setRange(0, 255, 1);
    _rSlider.setTextValueSuffix(" R");

    addAndMakeVisible(_rLabel);
    _rLabel.setText("RED", juce::NotificationType::dontSendNotification);
    _rLabel.attachToComponent(&_rSlider, true);
    _rSlider.addListener(this);
    // *Red Slider

    // Green Slider
    addAndMakeVisible(_gSlider);
    _gSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    _gSlider.setRange(0, 255, 1);
    _gSlider.setTextValueSuffix(" G");

    addAndMakeVisible(_gLabel);
    _gLabel.setText("GREEN", juce::NotificationType::dontSendNotification);
    _gLabel.attachToComponent(&_gSlider, true);
    _gSlider.addListener(this);
    // *Green Slider

    // Blue Slider
    addAndMakeVisible(_bSlider);
    _bSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    _bSlider.setRange(0, 255, 1);
    _bSlider.setTextValueSuffix(" B");

    addAndMakeVisible(_bLabel);
    _bLabel.setText("BLUE", juce::NotificationType::dontSendNotification);
    _bLabel.attachToComponent(&_bSlider, true);
    _bSlider.addListener(this);
    // *Blue Slider

    // Color Correct Slider Labels
    if (_restHandler.getG() > 180 || (_restHandler.getG() > 150 && _restHandler.getB() > 150)) {
        g.setColour(juce::Colours::black);

        _rLabel.setColour(juce::Label::textColourId, juce::Colours::black);
        _rSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::black);
        _gLabel.setColour(juce::Label::textColourId, juce::Colours::black);
        _gSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::black);
        _bLabel.setColour(juce::Label::textColourId, juce::Colours::black);
        _bSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::black);
    }
    else {
        g.setColour(juce::Colours::white);
        _rLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        _rSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::white);
        _gLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        _gSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::white);
        _bLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        _bSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::white);
    }

    g.setFont(30.0f);
    g.drawText(_currentSizeAsString, getLocalBounds(),
        juce::Justification::centred, true);

    // Mucking Around
}

void RGBSlider::resized()
{
    size();
    _currentSizeAsString = juce::String (getWidth()) + " x " + juce::String (getHeight());

    int height = getHeight() - getHeight() / 1.5;

    const int border = 100;
    // Resize Sliders
    _rSlider.setBounds(border, height, getWidth() - border, 20);
    _gSlider.setBounds(border, height + 20, getWidth() - border, 20);
    _bSlider.setBounds(border, height + 40, getWidth() - border, 20);
    // *Resize Sliders

    // Mucking Around
}

void RGBSlider::size() {
    setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
}

// Runs when slider value is changed
void RGBSlider::sliderValueChanged(juce::Slider* slider) {
    if (slider == &_rSlider) {
        _restHandler.setR(_rSlider.getValue());
    }
    else if (slider == &_gSlider) {
        _restHandler.setG(_gSlider.getValue());
    }
    else {
        _restHandler.setB(_bSlider.getValue());
    }
}

// Runs when mouse is lifted from a slider
void RGBSlider::sliderDragEnded(juce::Slider* slider) {
    // _restHandler.getPhilipsData();
    _restHandler.grabRGBPushUpdate();
}