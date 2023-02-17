/*
  ==============================================================================

    Main_C.cpp
    Created: 9 Oct 2022 12:05:31am
    Author:  Tiparium

  ==============================================================================
*/

#include "Main_C.h"

Main_C::Main_C() :
    _webServerHandler(&_ledRGB, &_rgbPattern),
    _pHuePHueHandler(params::_httpTarget, params::_apiTarget, params::_apiGetTarget, params::_apiPutTarget),
    _persistenceJSONHandler("../../resources/favSlots.json")
{

    TIP_RGB r = TIP_RGB(255, 0, 0);
    TIP_RGB g = TIP_RGB(0, 255, 0);
    TIP_RGB b = TIP_RGB(0, 0, 255);
    _rgbPattern.push_back(r);
    _rgbPattern.push_back(g);
    _rgbPattern.push_back(b);


    _panelSelector = new juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop);
    addAndMakeVisible(_panelSelector);

    // Build the UI
    _coreLEDControlPanel = new CoreLEDControlPanel(
        dynamic_cast<juce::Component*>(this),
        &_webServerHandler, &_pHuePHueHandler,
        &_persistenceJSONHandler,
        &_uiRGB,
        &_ledRGB);
    _panelSelector->addTab("Home", juce::Colours::slategrey, _coreLEDControlPanel, true, 0);

    _nodeMCUPatternProgrammer = new NodeMCUPatternProgrammer(
        dynamic_cast<juce::Component*>(this),
        &_webServerHandler, &_pHuePHueHandler,
        &_persistenceJSONHandler,
        &_uiRGB, 
        &_ledRGB,
        &_patternOverrideMode);
    _panelSelector->addTab("Pattern Programmer", juce::Colours::slategrey, _nodeMCUPatternProgrammer, true, 1);

    _coreLEDControlPanel->setSliderValues(_uiRGB);

    // Start NodeMCU Broadcast thread
    _webServerHandler.startThread();
}

Main_C::~Main_C()
{
    removeAllChildren();
    delete _coreLEDControlPanel;
    delete _nodeMCUPatternProgrammer;
    delete _panelSelector;

    _persistenceJSONHandler.saveJSONToFile();
}

void Main_C::handleCommandMessage(int commandId)
{
    switch (commandId)
    {
    case tip_rgb_values_updated:
        repaint();
        break;

    default:
        throw std::invalid_argument("Invalid Command Message");
        break;
    }
}

void Main_C::paint(juce::Graphics& g)
{
    TIP_RGB currentRGB = _uiRGB.colorCorrect();
    _panelSelector->setColour(juce::TabbedComponent::ColourIds::backgroundColourId,
        juce::Colour(currentRGB.r, currentRGB.g, currentRGB.b));
}
void Main_C::resized()
{
    juce::Rectangle<int> bounds = getBounds();
    bounds.setPosition(0, 0);
    _panelSelector->setBounds(bounds);
}