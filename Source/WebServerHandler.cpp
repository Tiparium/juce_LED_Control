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

void WebServerHandler::pingServer()
{
    DBG("Attempting SHUTDOWN Ping...");
    adamski::RestRequest::Response res = _req.get(_addr).execute();
    DBG("SHUTDOWN Ping Received!");
}