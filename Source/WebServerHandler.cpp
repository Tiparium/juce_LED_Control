/*
  ==============================================================================

    WebServerHandler.cpp
    Created: 18 Nov 2022 4:03:19pm
    Author:  Tiparium

  ==============================================================================
*/

#include "WebServerHandler.h"


WebServerHandler::WebServerHandler(TIP_RGB* rgbRef, std::vector<TIP_RGB>* rgbPatternRef) :
    _rgbRef(rgbRef),
    _rgbPatternRef(rgbPatternRef),
    juce::Thread("NodeMCU RestAPI Thread")
{
}

WebServerHandler::~WebServerHandler()
{
    stopThread(_timeout + 500);
}

void WebServerHandler::run()
{
    while (!threadShouldExit())
    {
        if (_localRGBPattern.empty())
        {
            _localRGBPattern.push_back(TIP_RGB(255, 255, 255));
        }
        if (!_stateOverride)
        {
            // All LEDS on NodeMCU should be the same color
            if (!_localRGBPattern[0].isEqualTo((*_rgbRef).colorCorrect()))
            {
                _localRGBPattern[0] = (*_rgbRef).colorCorrect();
                std::vector<TIP_RGB> temp;
                temp.push_back(_localRGBPattern[0]);
                sendRGBPatternPostRequest(temp);
            }
        }
        else
        {
            if (!checkRGBPatternEquality(_localRGBPattern, _rgbPatternRef))
            {
                _localRGBPattern = *_rgbPatternRef;
                sendRGBPatternPostRequest(_localRGBPattern);
            }
        }


    }
}

void WebServerHandler::sendSingleRGBPostRequest(TIP_RGB rgb)
{
    juce::URL url = juce::URL(_addr);
    juce::StringPairArray responseHeaders;
    int statusCode = 0;

    // Build JSON
    nlohmann::json json;
    json["r"] = rgb.r;
    json["g"] = rgb.g;
    json["b"] = rgb.b;
    nlohmann::json jsonArr;
    jsonArr["rgbDataPoints"].push_back(json);

    juce::String jsonAsStr = jsonArr.dump();
    DBG(jsonAsStr);

    if (!jsonAsStr.isEmpty()) { url = url.withPOSTData(jsonAsStr); }
    else { DBG("Something went VERY wrong here, RGB values should always be populated."); }

    std::unique_ptr<juce::InputStream> stream(url.createInputStream(
        true, nullptr, nullptr,
        { "Content-Type: application/json" }, _timeout,
        &responseHeaders, &statusCode, 5, "POST"));

    bool debug = false;

    if (debug || _classDebug)
    {
        if (statusCode == 0)
        {
            DBG("NodeMCU Request Timed Out");
        }
        else
        {
            DBG("NodeMCU Request Succeeded!");
        }
    }
}

void WebServerHandler::sendRGBPatternPostRequest(std::vector<TIP_RGB> pattern)
{
    juce::URL url = juce::URL(_addr);
    juce::StringPairArray responseHeaders;
    int statusCode = 0;
    nlohmann::json jsonArr;

    for (int i = 0; i < pattern.size(); i++)
    {
        TIP_RGB rgb = pattern[i];
        nlohmann::json json;

        json["r"] = rgb.r;
        json["g"] = rgb.g;
        json["b"] = rgb.b;

        jsonArr["rgbDataPoints"].push_back(json);
    }

    juce::String jsonAsStr = jsonArr.dump();
    DBG(jsonAsStr);

    if (!jsonAsStr.isEmpty()) { url = url.withPOSTData(jsonAsStr); }
    else { DBG("Something went VERY wrong here, RGB values should always be populated."); }

    std::unique_ptr<juce::InputStream> stream(url.createInputStream(
        true, nullptr, nullptr,
        { "Content-Type: application/json" }, _timeout,
        &responseHeaders, &statusCode, 5, "POST"));

    bool debug = false;

    if (debug || _classDebug)
    {
        if (statusCode == 0)
        {
            DBG("NodeMCU Request Timed Out");
        }
        else
        {
            DBG("NodeMCU Request Succeeded!");
        }
    }
}

bool WebServerHandler::checkRGBPatternEquality(std::vector<TIP_RGB> local, std::vector<TIP_RGB>* newData)
{
    if (local.size() != newData->size())
    {
        return false;
    }
    std::vector<TIP_RGB> localCopyOfNewData = *newData;
    for (int i = 0; i < local.size(); i++)
    {
        if (!local[i].isEqualTo(localCopyOfNewData[i]))
        {
            return false;
        }
    }
    return true;
}

// G/S
void WebServerHandler::setRGBPatternRef(std::vector<TIP_RGB>* patternRef)
{ _rgbPatternRef = patternRef; }
// *G/S