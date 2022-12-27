/*
  ==============================================================================

    NodeMCUPatternProgrammer.cpp
    Created: 26 Dec 2022 5:23:42pm
    Author:  Tiparium

  ==============================================================================
*/

#include "NodeMCUPatternProgrammer.h"

NodeMCUPatternProgrammer::NodeMCUPatternProgrammer(juce::Component* parent) :
    _parent(parent)
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

void NodeMCUPatternProgrammer::setWebServerHandlerRef(WebServerHandler* nodeMCUServerHandlerRef)
{ _nodeMCUServerHandlerRef = nodeMCUServerHandlerRef; }
void NodeMCUPatternProgrammer::setPersistenceHandlerRef(PersistenceJSONHandler* favsHandlerRef)
{ _favsHandlerRef = favsHandlerRef; }

// *G/S