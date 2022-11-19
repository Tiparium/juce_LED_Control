/*
  ==============================================================================

    BasicWebServer.cpp
    Created: 6 Nov 2022 4:16:12pm
    Author:  Tiparium

  ==============================================================================
*/

#include "WebServerWrapper.h"

WebServerWrapper::WebServerWrapper(juce::String url, int port) :
    _url(url),
    _port(port),
    _shouldRun(true),
    Thread("WebServerThread"),
    _mcc(_url.toStdString().c_str(), _port, &_shouldRun),
    _handler(_url + ":" + juce::String(_port))
{
}

void WebServerWrapper::stopBasicWebServer(float timeout)
{
    _shouldRun = false;
    _handler.pingServer();
    // Anything which must be done with the webserver before stopping it, do it here.
    stopThread(timeout);
}

void WebServerWrapper::run()
{
    if (_mcc.init() != 0) {
        return;
    }
    _mcc.run();
    while (!threadShouldExit())
    {

    }
}