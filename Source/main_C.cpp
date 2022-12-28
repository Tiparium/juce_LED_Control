/*
  ==============================================================================

    Main_C.cpp
    Created: 9 Oct 2022 12:05:31am
    Author:  Tiparium

    Contains & Displays subcomponents both for data processing & UI display.

  ==============================================================================
*/

#include "Main_C.h"

Main_C::Main_C() :
    _coreLEDControlPanel()
{
    _panelSelector = new juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop);
    // _panelSelector->setBounds(0, 0, 300, getHeight());
    addAndMakeVisible(_panelSelector);

    // Build the UI
    _coreLEDControlPanel = new CoreLEDControlPanel(dynamic_cast<juce::Component*>(this));
    _panelSelector->addTab("Home", juce::Colours::slategrey, _coreLEDControlPanel, true, 0);

    _nodeMCUPatternProgrammer = new NodeMCUPatternProgrammer(dynamic_cast<juce::Component*>(this));
    _nodeMCUPatternProgrammer->setWebServerHandlerRef(_coreLEDControlPanel->getWebServerHandlerRef());
    _nodeMCUPatternProgrammer->setPersistenceHandlerRef(_coreLEDControlPanel->getPersistenceHandlerRef());
    _panelSelector->addTab("Pattern Programmer", juce::Colours::slategrey, _nodeMCUPatternProgrammer, true, 1);
    
    setSize(800, 600);
}

Main_C::~Main_C()
{
    removeAllChildren();
    delete _coreLEDControlPanel;
    delete _nodeMCUPatternProgrammer;
    delete _panelSelector;
}

void Main_C::handleCommandMessage(int commandId)
{
    switch (commandId)
    {
    case tip_rgb_Values_Updated:
        repaint();
        break;

    default:
        throw std::invalid_argument("Invalid Command Message");
        break;
    }
}

void Main_C::paint(juce::Graphics& g)
{
    TIP_RGB currentRGB = _coreLEDControlPanel->getTempRGB().colorCorrect();
    _panelSelector->setColour(juce::TabbedComponent::ColourIds::backgroundColourId,
        juce::Colour(currentRGB.r, currentRGB.g, currentRGB.b));
}
void Main_C::resized()
{
    juce::Rectangle<int> bounds = getBounds();
    bounds.setPosition(0, 0);
    _panelSelector->setBounds(bounds);
}