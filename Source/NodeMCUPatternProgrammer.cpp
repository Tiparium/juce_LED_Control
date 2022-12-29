/*
  ==============================================================================

    NodeMCUPatternProgrammer.cpp
    Created: 26 Dec 2022 5:23:42pm
    Author:  Tiparium

  ==============================================================================
*/

#include "NodeMCUPatternProgrammer.h"

NodeMCUPatternProgrammer::NodeMCUPatternProgrammer(juce::Component* parent, WebServerHandler* webServerHandler, PHueHandler* pHueHandler, PersistenceJSONHandler* persistenceJSONHandler, TIP_RGB* uiRGB, TIP_RGB* ledRGB) :
    _webServerHandler_Ref(webServerHandler),
    _pHuePHueHandler_Ref(pHueHandler),
    _persistenceJSONHandler_Ref(persistenceJSONHandler),
    _parent(parent),
    _uiRGB_Ref(uiRGB),
    _ledRGB_Ref(ledRGB)
{
}

NodeMCUPatternProgrammer::~NodeMCUPatternProgrammer()
{
    // Handle persistance

    // Remove listeners

    // Delete any dynamic variables
}

void NodeMCUPatternProgrammer::sliderValueChanged(juce::Slider* slider)
{

}

void NodeMCUPatternProgrammer::sliderDragEnded(juce::Slider* slider)
{

}

void NodeMCUPatternProgrammer::buttonClicked(juce::Button* button)
{

}

// G/S
// *G/S