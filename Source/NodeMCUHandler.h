/*
  ==============================================================================

    NodeMCUHandler.h
    Created: 6 Nov 2022 12:19:12pm
    Author:  Tiparium

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "../Source/MultiClientChat.h"

class NodeMCUHandler : public juce::Thread
{
public:
    NodeMCUHandler() : Thread("NodeMCUThread") {}

    void stopNodeMCUHandler(float timeout);

private:
    void run() override {
        MultiClientChat _mcc("0.0.0.0", 8080);
        if (_mcc.init() != 0) {
            return;
        }
        _mcc.run();
        while (!threadShouldExit())
        {

        }
    }
};