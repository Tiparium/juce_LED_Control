/*
  ==============================================================================

    RestHandler.cpp
    Created: 25 Dec 2022 4:49:48pm
    Author:  Tiparium

  ==============================================================================
*/

#include "RestHandler.h"

RestHandler::RestHandler(int id, TIP_RGB* rgbRef, juce::String addr) :
    _id(id),
    _rgbRef(rgbRef),
    _addr(addr),
    Thread("Rest Handler Thread: " + std::to_string(id))
{
}

RestHandler::~RestHandler() {}