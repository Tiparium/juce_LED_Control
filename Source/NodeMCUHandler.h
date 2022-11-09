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
#include "../Source/RGBStructs.h"

class NodeMCUHandler : public juce::Thread
{
public:
    NodeMCUHandler() : Thread("NodeMCUThread") {};
    void stopNodeMCUHandler(float timeout);
    void setRGB(TIP_RGB rgbIn);
private:
    crow::SimpleApp _app;
    TIP_RGB _rgb;
    void run() override {
        CROW_ROUTE(_app, "/rgb/")([&]() {
            crow::json::wvalue val({
                "r", _rgb.r,
                "g", _rgb.g,
                "b", _rgb.b
            });
            return val;
        });
        _app.port(18080).run();
        while (!threadShouldExit()) {

            CROW_ROUTE(_app, "/rgb/")([&]() {
                crow::json::wvalue val({
                    "r", _rgb.r,
                    "g", _rgb.g,
                    "b", _rgb.b
                });
                return val;
            });

            sleep(10);
        }
    }
};