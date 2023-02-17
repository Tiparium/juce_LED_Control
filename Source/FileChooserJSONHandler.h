/*
  ==============================================================================

    FileChooserJSONHandler.h
    Created: 4 Feb 2023 8:43:28am
    Author:  Tiparium

    Handles opening & saving files via native explorer dialogue.
    
    TODO: See about merging this class with persistence handler.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "nlohmann/json.hpp"
#include "../Source/RGBStructs.h"
#include "../Source/PatternPickerSlot.h"
#include <fstream>

class FileChooserJSONHandler
{
public:
    FileChooserJSONHandler();
    ~FileChooserJSONHandler();

    void           writeDataToJSONFile(std::vector<PatternPickerSlot*> pattern, int multiplier);
    nlohmann::json readDataFromJSONFile();
    // DBG
    void holder();
private:
    std::unique_ptr<juce::FileChooser> _fileChooser;
};