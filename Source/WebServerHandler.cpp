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


WebServerHandler::WebServerHandler()
{
    _req.header("Content-Type", "application/json");
    _req.header("Authorization", "Basic " + juce::Base64::toBase64("username:password"));
}

WebServerHandler::~WebServerHandler() {}

void WebServerHandler::pushToServer(TIP_RGB rgb)
{
    pushToServer_NativeJUCE(rgb);
}

void WebServerHandler::pushToServer_NativeJUCE(TIP_RGB rgb)
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
    
    if (!jsonAsStr.isEmpty()){ url = url.withPOSTData(jsonAsStr); }
    else { DBG("Something went VERY wrong here, RGB values should always be populated."); }
    
    std::unique_ptr<juce::InputStream> stream(url.createInputStream(
        true, nullptr, nullptr,
        { "Content-Type: application/json" }, _timeout,
        &responseHeaders, &statusCode, 5, "POST"));
}