/*
  ==============================================================================

    WebServerThreadWrapper.h
    Created: 18 Dec 2022 7:04:55pm
    Author:  Tiparium

    Wraps NodeMCU / Arduino WebServer in a thread, so the program no longer
        hangs on updates to the color update calls to the webserver when it
        is inactive.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../Source/WebServerHandler.h"

class WebServerThreadWrapper : public juce::Thread
{
public:

    WebServerThreadWrapper(TIP_RGB* rgbRef);
    ~WebServerThreadWrapper() override;

    bool compareRGB(TIP_RGB newRGB, TIP_RGB oldRGB);
    void run() override;

private:
    TIP_RGB*     _rgbRef;
    TIP_RGB      _currRGB;

    // Talk to the Handlers
    WebServerHandler        _nodeMCUServerHandler;
};