/*
  ==============================================================================

    NodeMCUHandler.h
    Created: 6 Nov 2022 12:19:12pm
    Author:  Tiparium

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "./crow.h"

class NodeMCUHandler
{
public:
    NodeMCUHandler();
    ~NodeMCUHandler();
private:
    crow::SimpleApp app;
};