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


WebServerHandler::WebServerHandler(juce::String addr):
    _addr(addr)
{
    _req.header("Content-Type", "application/json");
    _req.header("Authorization", "Basic " + juce::Base64::toBase64("username:password"));
}

WebServerHandler::~WebServerHandler() {}

void WebServerHandler::pushToServer(TIP_RGB rgb)
{
    // Build JSON data
    juce::uint8 r = rgb.r;
    juce::uint8 g = rgb.g;
    juce::uint8 b = rgb.b;

    nlohmann::json outputAsJson;
    juce::var rVar;
    juce::var gVar;
    juce::var bVar;

    rVar.append(std::to_string(r).c_str());
    gVar.append(std::to_string(g).c_str());
    bVar.append(std::to_string(b).c_str());

    std::string output = outputAsJson.dump();

    // Make RESTful PUT call
    adamski::RestRequest::Response res = _req.post(_addr)
        .field("r", std::to_string(r).c_str())
        .field("g", std::to_string(g).c_str())
        .field("b", std::to_string(b).c_str())
        .execute();

    bool debug = true;
    if (debug)
    {
        DBG(res.bodyAsString);
    }
}