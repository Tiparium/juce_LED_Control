/*
  ==============================================================================

    WebServerThreadWrapper.cpp
    Created: 18 Dec 2022 7:04:55pm
    Author:  Tiparium

  ==============================================================================
*/

#include "WebServerThreadWrapper.h"

WebServerThreadWrapper::WebServerThreadWrapper(TIP_RGB* rgbRef) :
    _rgbRef(rgbRef),
    _nodeMCUServerHandler(),
    juce::Thread("NodeMCUThread")
{
    startThread();
}

WebServerThreadWrapper::~WebServerThreadWrapper()
{
    stopThread(2000);
}

bool WebServerThreadWrapper::compareRGB(TIP_RGB newRGB, TIP_RGB oldRGB)
{
    if (newRGB.r == oldRGB.r && newRGB.g == oldRGB.g && newRGB.b == oldRGB.b)
        return true;
    return false;
}

void WebServerThreadWrapper::run()
{
    while (!threadShouldExit())
    {
        wait(50);

        if (!compareRGB(*_rgbRef, _currRGB))
        {
            _currRGB = *_rgbRef;
            _nodeMCUServerHandler.pushToServer(_currRGB);
        }
    }
}