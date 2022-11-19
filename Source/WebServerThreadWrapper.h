/*
  ==============================================================================

    WebServerThreadWrapper.h
    Created: 6 Nov 2022 12:19:12pm
    Author:  Tiparium

    A basic wrapper around a WebServer instance.
    The WebServer is run in it's own thread, and is shut down cleanly upon program close.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "../Source/WebServer.h"
#include "../Source/WebServerHandler.h"

class WebServerThreadWrapper : public juce::Thread
{
public:

    WebServerThreadWrapper();
    void stopBasicWebServer(float timeout);

private:
    const juce::String  _header = "http://";
    const juce::String  _url  = "127.0.0.1";
    int                 _port = 8080;

    volatile bool       _shouldRun;
    WebServer           _mcc = WebServer(_url.toUTF8(), 8080, &_shouldRun);
    WebServerHandler    _handler = WebServerHandler((_header + _url + ":" + juce::String(_port)).toUTF8());

    void run() override;
};