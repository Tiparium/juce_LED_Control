/*
  ==============================================================================

    persistenceJSONHandler.h
    Created: 29 Oct 2022 2:52:54pm
    Author:  merje

  ==============================================================================
*/

#pragma once

#include <fstream>

#include <../JuceLibraryCode/JuceHeader.h>
#include "../json/include/nlohmann/json.hpp"

class PersistenceJSONHandler
{
public:
    PersistenceJSONHandler(juce::String path);
    ~PersistenceJSONHandler();

    nlohmann::json readJSONFromFile();
    void saveJSONToFile(nlohmann::json jsonIn);

private:
    juce::String _path;
    juce::String _rootName;
};