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
    TIP_RGB r = TIP_RGB(255, 0, 0);
    TIP_RGB g = TIP_RGB(0, 255, 0);
    TIP_RGB b = TIP_RGB(0, 0, 255);
    _rgbPattern.push_back(r);
    _rgbPattern.push_back(g);
    _rgbPattern.push_back(b);
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
std::vector<TIP_RGB>* NodeMCUPatternProgrammer::getPatternRef()
{ return &_rgbPattern; }
// *G/S