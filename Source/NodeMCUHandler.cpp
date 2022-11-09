/*
  ==============================================================================

    NodeMCUHandler.cpp
    Created: 6 Nov 2022 4:16:12pm
    Author:  Tiparium

  ==============================================================================
*/

#include "NodeMCUHandler.h"

void NodeMCUHandler::stopNodeMCUHandler(float timeout)
{
    // Anything which must be done with the webserver before stopping it, do it here.

    _app.stop();
    stopThread(timeout);
}

void NodeMCUHandler::setRGB(TIP_RGB rgbIn)
{
    _rgb = rgbIn;
}