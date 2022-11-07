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

class NodeMCUHandler : public juce::Thread
{
public:
    NodeMCUHandler() : Thread("NodeMCUThread") {};
    void stopNodeMCUHandler(float timeout);
private:
    crow::SimpleApp _app;
    void run() override {
        CROW_ROUTE(_app, "/")([&]() {
            return "Hello World";
        });
        _app.port(18080).multithreaded().run();
        while (!threadShouldExit()) {
            int i = 0;
            sleep(10);
        }
    }
};