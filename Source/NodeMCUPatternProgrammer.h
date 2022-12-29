/*
  ==============================================================================

    NodeMCUPatternProgrammer.h
    Created: 26 Dec 2022 5:23:42pm
    Author:  Tiparium

    Allows uploading of specific patterns to the NodeMCU,
        as well as toggling other features.

    Makes changes to NodeMCU via reference to CoreLEDControlPanel's handler.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../Source/PersistenceJSONHandler.h"
#include "../Source/WebServerHandler.h"

class NodeMCUPatternProgrammer :
    public juce::Component,
    public juce::Slider::Listener,
    public juce::Button::Listener
{
public:

    NodeMCUPatternProgrammer(juce::Component* parent);
    ~NodeMCUPatternProgrammer();

    // RGBSliders
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;

    // Button Logic
    void buttonClicked(juce::Button* button) override;

    // G/S
    void setWebServerHandlerRef(WebServerHandler* nodeMCUServerHandlerRef);
    void setPersistenceHandlerRef(PersistenceJSONHandler* favsHandlerRef);
    std::vector<TIP_RGB>* getPatternRef();
    // *G/S

private:
    // TODO: Restructure so all of this stuff is stored in Main_C instead of here.
    //       There is no good reason NodeMCUPatternProgrammer should have them.
    //       (Other than that it made sense with my earlier design choices.)
    std::vector<TIP_RGB>        _rgbPattern;
    WebServerHandler*           _nodeMCUServerHandlerRef;
    PersistenceJSONHandler*     _favsHandlerRef;
    juce::Component* _parent;
};