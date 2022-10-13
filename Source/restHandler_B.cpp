/*
  ==============================================================================

    RestHandler_B.cpp
    Created: 12 Oct 2022 9:20:59pm
    Author:  Tiparium

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RestHandler_B.h"

RestHandler_B::RestHandler_B() {
    _req.header("Content-Type", "application/json");
    _req.header("Authorization", "Basic " + juce::Base64::toBase64("username:password"));

    _httpTarget = params::_httpTarget;
    _apiTarget = params::_apiTarget;
    _apiGetTarget = params::_apiGetTarget;
    _apiPutTarget = params::_apiPutTarget;

    RestHandler_B::updateRootJSON();
    _numLights = RestHandler_B::_rootJSON.size();

    // These aren't needed for the app to run, just for resetColor()
    _OGxVal = _rootJSON["1"]["state"]["xy"][0];
    _OGyVal = _rootJSON["1"]["state"]["xy"][1];
}

RestHandler_B::~RestHandler_B() {
    resetColor();
}

void RestHandler_B::getPhilipsData() {

    for (int i = 0; i < _rootJSON.size(); i++) {

        std::string it = std::to_string(i + 1);

        DBG(_rootJSON.size());

        float test = _rootJSON[it]["state"]["xy"][1];

        DBG(test);
    }

    //DBG(res.bodyAsString); // Prints the full response as JSON (not pretty)
    //DBG(res.result.getErrorMessage()); // Prints any errors that may occur
}

void RestHandler_B::grabRGBPushUpdate() {
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
        RGB rgb;
        XYBrightness xyb;
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

void RestHandler_B::pushUpdate(juce::var xyColor, juce::String target) {
    // Make RESTful PUT call
    adamski::RestRequest::Response res = _req.put(target)
        .field("xy", xyColor)
        .execute();

    DBG(res.bodyAsString);
}

void RestHandler_B::updateRootJSON() {
    juce::String target = _httpTarget + _apiTarget + _apiGetTarget;
    DBG(target);
    _rootJSON = pingAndReceive(target);
}

nlohmann::json RestHandler_B::pingAndReceive(juce::String target) {
    // DBG(target); // Prints the target URL
    adamski::RestRequest::Response res = _req.get(target).execute();

    juce::String resBody = res.bodyAsString;
    // json::jobject result = json::jobject::parse(resBody.toStdString());
    nlohmann::json json = nlohmann::json::parse(resBody.toStdString());
    return json;
}

int RestHandler_B::getNumLights() {
    return _rootJSON.size();
}

/**
EVERYTHING BELOW THIS LINE IS FOR DEBUG PURPOSES ONLY
*/
void RestHandler_B::resetColor() {
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

// Getters / Setters
void RestHandler_B::setR(juce::uint8 val) { _rVal = val; }
void RestHandler_B::setG(juce::uint8 val) {_gVal = val;}
void RestHandler_B::setB(juce::uint8 val) {_bVal = val;}
juce::uint8 RestHandler_B::getR() { return _rVal;}
juce::uint8 RestHandler_B::getG() { return _gVal;}
juce::uint8 RestHandler_B::getB() { return _bVal;}
//* Getters / Setters