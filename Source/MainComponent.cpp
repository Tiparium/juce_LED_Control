/*
  ==============================================================================

    MainComponent.cpp
    Created: 26 Sep 2022 11:09:01am
    Author:  Tiparium

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    _req.header("Content-Type", "application/json");
    _req.header("Authorization", "Basic " + juce::Base64::toBase64("username:password"));

    _httpTarget = params::_httpTarget;
    _apiTarget = params::_apiTarget;
    _apiGetTarget = params::_apiGetTarget;
    _apiPutTarget = params::_apiPutTarget;

    MainComponent::updateRootJSON();
    _numLights = MainComponent::_rootJSON.size();

    _OGxVal = _rootJSON["1"]["state"]["xy"][0];
    _OGyVal = _rootJSON["1"]["state"]["xy"][1];
}

MainComponent::~MainComponent()
{
    resetColor();
}

void MainComponent::paint (juce::Graphics& g)
{
    setSize(600, 400);
    size();
    g.drawRect(getLocalBounds(), 1);
    g.fillAll (juce::Colour(_rVal, _gVal, _bVal)); // Set the color of the window to be the current RGB value

    g.setColour (juce::Colours::grey);

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

    if (_gVal > 180 || (_gVal > 150 && _bVal > 150)) {
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

    g.setFont (30.0f);
    g.drawText (_currentSizeAsString, getLocalBounds(),
                juce::Justification::centred, true);
}

void MainComponent::resized()
{
    size();
    //_currentSizeAsString = juce::String (getWidth()) + " x " + juce::String (getHeight());

    int height = getHeight() - getHeight() / 1.5;

    const int border = 100;
    // Resize Sliders
    _rSlider.setBounds(border, height, getWidth() - border, 20);
    _gSlider.setBounds(border, height + 20, getWidth() - border, 20);
    _bSlider.setBounds(border, height + 40, getWidth() - border, 20);
    // *Resize Sliders
}

void MainComponent::size() {
    setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
}

// Runs when slider value is changed
void MainComponent::sliderValueChanged(juce::Slider* slider) {
    if (slider == &_rSlider) {
        _rVal = _rSlider.getValue();
    }
    else if (slider == &_gSlider) {
        _gVal = _gSlider.getValue();
    }
    else {
        _bVal = _bSlider.getValue();
    }
}

// Runs when mouse is lifted from a slider
void MainComponent::sliderDragEnded(juce::Slider* slider) {
    // MainComponent::getPhilipsData();
    MainComponent::grabRGBPushUpdate();
}

void MainComponent::getPhilipsData() {

    for (int i = 0; i < _rootJSON.size(); i++) {

        std::string it = std::to_string(i + 1);

        DBG(_rootJSON.size());

        float test = _rootJSON[it]["state"]["xy"][1];

        DBG(test);
    }

    //DBG(res.bodyAsString); // Prints the full response as JSON (not pretty)
    //DBG(res.result.getErrorMessage()); // Prints any errors that may occur
}

void MainComponent::grabRGBPushUpdate() {
    juce::String putTarget = _apiPutTarget;
    juce::String target = "";
    for (int i = 0; i < _numLights; i++) {

        // Build a target URL unique to each pHue LED
        putTarget = putTarget.substring(0, _apiPutSplit) +
            std::to_string(i + 1) +
            putTarget.substring(_apiPutSplit + 1, putTarget.length());

        target = _httpTarget + _apiTarget + putTarget;

        DBG("Target Value: " + target);

        // Convert our native RGB values to pHueXY
        MainComponent::RGB rgb;
        MainComponent::XYBrightness xyb;
        rgb.r = _rVal;
        rgb.g = _gVal;
        rgb.b = _bVal;
        xyb = rgb.toXY();

        // Push our new pHueXY values to a juce::obj for RESTful call
        juce::var xyColor;
        xyColor.append(xyb.xy.x);
        xyColor.append(xyb.xy.y);

        // Perform RESTful call
        pushUpdate(xyColor, target);
    }
}

void MainComponent::pushUpdate(juce::var xyColor, juce::String target) {
    // Make RESTful PUT call
    adamski::RestRequest::Response res = _req.put(target)
        .field("xy", xyColor)
        .execute();

    DBG(res.bodyAsString);
}

void MainComponent::updateRootJSON() {
    juce::String target = _httpTarget + _apiTarget + _apiGetTarget;
    DBG(target);
    _rootJSON = pingAndReceive(target);
}

nlohmann::json MainComponent::pingAndReceive(juce::String target) {
    // DBG(target); // Prints the target URL
    adamski::RestRequest::Response res = _req.get(target).execute();

    juce::String resBody = res.bodyAsString;
    // json::jobject result = json::jobject::parse(resBody.toStdString());
    nlohmann::json json = nlohmann::json::parse(resBody.toStdString());
    return json;
}

int MainComponent::getNumLights() {
    return _rootJSON.size();
}

/**

EVERYTHING BELOW THIS LINE IS FOR DEBUG PURPOSES ONLY

*/
void MainComponent::resetColor() {
    juce::var xyColor;
    xyColor.append(_OGxVal);
    xyColor.append(_OGyVal);

    juce::String putTarget = _apiPutTarget;
    juce::String target = "";

    for (int i = 0; i < _numLights; i++) {
        int it = i + 1;
        // Build a target URL unique to each pHue LED
        putTarget = putTarget.substring(0, _apiPutSplit) +
            std::to_string(i + 1) +
            putTarget.substring(_apiPutSplit + 1, putTarget.length());

        target = _httpTarget + _apiTarget + putTarget;

        pushUpdate(xyColor, target);
    }
}