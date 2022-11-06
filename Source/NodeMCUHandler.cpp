/*
  ==============================================================================

    NodeMCUHandler.cpp
    Created: 6 Nov 2022 12:19:12pm
    Author:  Tiparium

  ==============================================================================
*/

#include "NodeMCUHandler.h"

NodeMCUHandler::NodeMCUHandler()
{
    CROW_ROUTE(app, "/")([]() {
        return "Hello World!";
    });
    app.port(18080).multithreaded().run();
}

NodeMCUHandler::~NodeMCUHandler()
{
    app.stop();
}