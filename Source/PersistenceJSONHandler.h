/*
  ==============================================================================

    persistenceJSONHandler.h
    Created: 29 Oct 2022 2:52:54pm
    Author:  Tiparium

  ==============================================================================
*/

#pragma once

#include <fstream>

#include "../Source/PersistenceIDValues.h"
#include <../JuceLibraryCode/JuceHeader.h>
#include "../json/include/nlohmann/json.hpp"

class PersistenceJSONHandler
{
public:
    PersistenceJSONHandler(juce::String path);
    ~PersistenceJSONHandler();

    void checkForEmptyJSON();
    nlohmann::json readJSONFromFile();
    void addJSONToLocalInstance(juce::String segmentTitle, nlohmann::json jsonIn);
    void saveJSONToFile();

private:
    nlohmann::json  _localInstance;

    juce::String    _path;
    juce::String    _rootName;
};