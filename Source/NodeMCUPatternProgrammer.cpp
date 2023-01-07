/*
  ==============================================================================

    NodeMCUPatternProgrammer.cpp
    Created: 26 Dec 2022 5:23:42pm
    Author:  Tiparium

  ==============================================================================
*/

#include "NodeMCUPatternProgrammer.h"

NodeMCUPatternProgrammer::NodeMCUPatternProgrammer(juce::Component* parent, WebServerHandler* webServerHandler, PHueHandler* pHueHandler, PersistenceJSONHandler* persistenceJSONHandler, TIP_RGB* uiRGB, TIP_RGB* ledRGB) :
    _webServerHandler_Ref(webServerHandler),
    _pHuePHueHandler_Ref(pHueHandler),
    _persistenceJSONHandler_Ref(persistenceJSONHandler),
    _parent(parent),
    _uiRGB_Ref(uiRGB),
    _ledRGB_Ref(ledRGB)
{
}

NodeMCUPatternProgrammer::~NodeMCUPatternProgrammer()
{
    // Handle persistance

    // Remove listeners

    // Delete any dynamic variables
}

void NodeMCUPatternProgrammer::paint(juce::Graphics& g)
{
    paintSliders(g);
    paintPatternGenerator(g);
}

void NodeMCUPatternProgrammer::paintSliders(juce::Graphics& g)
{
    float relativeXPos;
    float relativeYPos;
    float relativeWidth;
    float relativeHeight;

    // Red Slider
    addAndMakeVisible(_rSlider);
    _rSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    _rSlider.setRange(0, 255, 1);
    _rSlider.setTextValueSuffix(" R");

    addAndMakeVisible(_rLabel);
    _rLabel.setText("RED", juce::NotificationType::dontSendNotification);
    _rLabel.attachToComponent(&_rSlider, true);
    _rSlider.addListener(this);
    //* Red Slider

    // Green Slider
    addAndMakeVisible(_gSlider);
    _gSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    _gSlider.setRange(0, 255, 1);
    _gSlider.setTextValueSuffix(" G");

    addAndMakeVisible(_gLabel);
    _gLabel.setText("GREEN", juce::NotificationType::dontSendNotification);
    _gLabel.attachToComponent(&_gSlider, true);
    _gSlider.addListener(this);
    //* Green Slider

    // Blue Slider
    addAndMakeVisible(_bSlider);
    _bSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    _bSlider.setRange(0, 255, 1);
    _bSlider.setTextValueSuffix(" B");

    addAndMakeVisible(_bLabel);
    _bLabel.setText("BLUE", juce::NotificationType::dontSendNotification);
    _bLabel.attachToComponent(&_bSlider, true);
    _bSlider.addListener(this);
    //* Blue Slider

    // Set labels to black
    g.setColour(juce::Colours::black);
    _rLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    _rSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::black);
    _gLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    _gSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::black);
    _bLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    _bSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::black);

    g.setFont(30.0f);

    // Size RGBSliders
    relativeHeight = getHeight() / 2;
    relativeYPos = 100;
    _rSlider.setBounds(relativeYPos, relativeHeight, getWidth() - 2 * relativeYPos, 20);
    _gSlider.setBounds(relativeYPos, relativeHeight + 20, getWidth() - 2 * relativeYPos, 20);
    _bSlider.setBounds(relativeYPos, relativeHeight + 40, getWidth() - 2 * relativeYPos, 20);
    //* RGBSliders
} // PaintSlider

void NodeMCUPatternProgrammer::paintPatternGenerator(juce::Graphics& g)
{

}

void NodeMCUPatternProgrammer::sliderValueChanged(juce::Slider* slider)
{

}

void NodeMCUPatternProgrammer::sliderDragEnded(juce::Slider* slider)
{

}

void NodeMCUPatternProgrammer::buttonClicked(juce::Button* button)
{

}

// G/S
// *G/S