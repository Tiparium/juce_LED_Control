/*
  ==============================================================================

    NOTES: None :)

    RestHandler.cpp
    Created: 12 Oct 2022 9:20:59pm
    Author:  Tiparium

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RestHandler.h"

RestHandler::RestHandler(juce::String http, juce::String api, juce::String get, juce::String put)
{
    _req.header("Content-Type", "application/json");
    _req.header("Authorization", "Basic " + juce::Base64::toBase64("username:password"));

    _httpTarget = http;
    _apiTarget = api;
    _apiGetTarget = get;
    _apiPutTarget = put;

    updateRootJSON();

    if (_debugMode)
    {
        // These aren't needed for the app to run, just for resetColor()
        _OGxVal = _rootJSON["1"]["state"]["xy"][0];
        _OGyVal = _rootJSON["1"]["state"]["xy"][1];
    }
}

RestHandler::~RestHandler()
{
    if (_debugMode)
    {
        resetColor();
    }
}

void RestHandler::pushUpdate(TIP_RGB rgb, int lightID)
{
    juce::String putTarget = _apiPutTarget;
    juce::String target = "";

    // Build a target URL unique to LED
    putTarget = putTarget.substring(0, _apiPutSplit) +
        std::to_string(lightID) +
        putTarget.substring(_apiPutSplit + 1, putTarget.length());

    if (lightID > _numLights)
    {
        DBG("Error, this light ID exceeds the available lights.");
        return;
    }

    target = _httpTarget + _apiTarget + putTarget;

    TIP_XYB xyb = rgb.toXY();
    juce::var xyColor;
    xyColor.append(xyb.xy.x);
    xyColor.append(xyb.xy.y);

    // Make RESTful PUT call
    adamski::RestRequest::Response res = _req.put(target)
        .field("xy", xyColor)
        .execute();

    bool localDebug = false;
    if (localDebug || _classDebug) {
        DBG(res.bodyAsString);
        DBG("Target Value: " + target);
    }
}

void RestHandler::pushUpdateToMultipleLights(TIP_RGB rgb, std::vector<bool> lights)
{
    for (int i = 1; i <= _numLights; i++)
    {
        if (lights[i - 1])
        {
            pushUpdate(rgb, i);
        }
    }
    updateRootJSON();
}

void RestHandler::updateRootJSON() {
    juce::String target = _httpTarget + _apiTarget + _apiGetTarget;
    adamski::RestRequest::Response res = _req.get(target).execute();

    juce::String resBody = res.bodyAsString;
    nlohmann::json json = nlohmann::json::parse(resBody.toStdString());
    _rootJSON = json;
    _numLights = _rootJSON.size();

    bool localDebug = false;
    if (localDebug || _classDebug)
    {
        DBG(target);
    }
}

// Getters / Setters
int             RestHandler::getNumLights() { return _numLights; }
juce::String    RestHandler::getLightNameByID(int id)
{
    return _rootJSON[std::to_string(id)]["name"];
}
//* Getters / Setters

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

        // Make RESTful PUT call
        adamski::RestRequest::Response res = _req.put(target)
            .field("xy", xyColor)
            .execute();

        bool localDebug = false;
        if (localDebug || _classDebug) {
            DBG(res.bodyAsString);
        }
    }
}