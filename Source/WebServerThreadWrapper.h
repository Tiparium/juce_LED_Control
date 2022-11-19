/*
  ==============================================================================

    WebServerThreadWrapper.h
    Created: 6 Nov 2022 12:19:12pm
    Author:  Tiparium

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "../Source/WebServer.h"
#include "../Source/WebServerHandler.h"

class WebServerThreadWrapper : public juce::Thread
{
public:

    WebServerThreadWrapper(juce::String url, int port);
    void stopBasicWebServer(float timeout);

private:

    juce::String        _url;
    int                 _port;

    volatile bool       _shouldRun;
    WebServer           _mcc;
    WebServerHandler    _handler;

    void run() override;
};