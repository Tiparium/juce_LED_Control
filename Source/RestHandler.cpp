/*
  ==============================================================================

    NOTES:
        TODO: Restructure this class. Function Call names make very little sense.

    RestHandler.cpp
    Created: 12 Oct 2022 9:20:59pm
    Author:  Tiparium

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RestHandler.h"

RestHandler::RestHandler() {
    _req.header("Content-Type", "application/json");
    _req.header("Authorization", "Basic " + juce::Base64::toBase64("username:password"));

    _httpTarget = params::_httpTarget;
    _apiTarget = params::_apiTarget;
    _apiGetTarget = params::_apiGetTarget;
    _apiPutTarget = params::_apiPutTarget;

    _rVal = 0;
    _gVal = 0;
    _bVal = 0;

    RestHandler::updateRootJSON();
    _numLights = RestHandler::_rootJSON.size();

    // These aren't needed for the app to run, just for resetColor()
    _OGxVal = _rootJSON["1"]["state"]["xy"][0];
    _OGyVal = _rootJSON["1"]["state"]["xy"][1];
}

RestHandler::~RestHandler() {
    resetColor();
}

void RestHandler::takeColorPushUpdate(RGB rgb) {
    setRGB(rgb);
    grabColorPushUpdate();
}

void RestHandler::grabColorPushUpdate() {
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
        RGB rgb(_rVal, _gVal, _bVal);
        pushRGBToPHue(rgb, target);
    }
}

void RestHandler::pushRGBToPHue(RGB rgb, juce::String target) {
    XYBrightness xyb = rgb.toXY();
    juce::var xyColor;
    xyColor.append(xyb.xy.x);
    xyColor.append(xyb.xy.y);
    pushXYBToPHue(xyColor, target);
}

void RestHandler::pushXYBToPHue(juce::var xyColor, juce::String target) {
    // Make RESTful PUT call
    adamski::RestRequest::Response res = _req.put(target)
        .field("xy", xyColor)
        .execute();

    DBG(res.bodyAsString);
}

void RestHandler::updateRootJSON() {
    juce::String target = _httpTarget + _apiTarget + _apiGetTarget;
    adamski::RestRequest::Response res = _req.get(target).execute();

    juce::String resBody = res.bodyAsString;
    nlohmann::json json = nlohmann::json::parse(resBody.toStdString());
    DBG(target);
    _rootJSON = json;
}

/**
EVERYTHING BELOW THIS LINE IS FOR DEBUG PURPOSES ONLY
*/
void RestHandler::resetColor() {
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

        pushXYBToPHue(xyColor, target);
    }
}

// Getters / Setters
void RestHandler::setRGB(RGB rgb) {
    _rVal = rgb._r;
    _gVal = rgb._g;
    _bVal = rgb._b;
}
RGB  RestHandler::getRGB() {
    RGB rgb = RGB(_rVal, _gVal, _bVal);
    return rgb;
}
void RestHandler::setR(juce::uint8 val) { _rVal = val; }
void RestHandler::setG(juce::uint8 val) {_gVal = val;}
void RestHandler::setB(juce::uint8 val) {_bVal = val;}
juce::uint8 RestHandler::getR() { return _rVal;}
juce::uint8 RestHandler::getG() { return _gVal;}
juce::uint8 RestHandler::getB() { return _bVal;}
int RestHandler::getNumLights() {return _rootJSON.size();}
//* Getters / Setters