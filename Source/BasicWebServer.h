/*
  ==============================================================================

    BasicWebServer.h
    Created: 6 Nov 2022 12:19:12pm
    Author:  Tiparium

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "../Source/MultiClientChat.h"
#include "../Source/WebServerHandler.h"

class BasicWebServer : public juce::Thread
{
public:

    BasicWebServer(juce::String url, int port);
    void stopBasicWebServer(float timeout);

private:

    juce::String        _url;
    int                 _port;

    volatile bool       _shouldRun;
    MultiClientChat     _mcc;
    WebServerHandler    _handler;

    void run() override;
};