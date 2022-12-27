/*
  ==============================================================================

    WebServerHandler.cpp
    Created: 18 Nov 2022 4:03:19pm
    Author:  Tiparium

    Client side of WebServer, used for sending / receiving data from the Server
    Also needed in order to shut the server down.

  ==============================================================================
*/

#include "WebServerHandler.h"


WebServerHandler::WebServerHandler(TIP_RGB* rgbRef) :
    _rgbRef(rgbRef),
    juce::Thread("NodeMCU RestAPI Thread")
{
    startThread();
}

WebServerHandler::~WebServerHandler()
{
    stopThread(_timeout + 500);
}

void WebServerHandler::run()
{
    while (!threadShouldExit())
    {
        if (!_localRGB.equals(_rgbRef->colorCorrect()))
        {
            _localRGB = _rgbRef->colorCorrect();
            sendPostRequest(_localRGB);
        }
    }
}

void WebServerHandler::sendPostRequest(TIP_RGB rgb)
{
    juce::URL url = juce::URL(_addr);
    juce::StringPairArray responseHeaders;
    int statusCode = 0;

    // Build JSON
    nlohmann::json json;
    json["r"] = rgb.r;
    json["g"] = rgb.g;
    json["b"] = rgb.b;
    juce::String jsonAsStr = json.dump();
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