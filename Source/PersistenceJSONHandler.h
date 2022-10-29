/*
  ==============================================================================

    persistenceJSONHandler.h
    Created: 29 Oct 2022 2:52:54pm
    Author:  merje

  ==============================================================================
*/

#pragma once

#include <../JuceLibraryCode/JuceHeader.h>
#include <fstream>
#include "../json/include/nlohmann/json.hpp"

class PersistenceJSONHandler
{
public:
    PersistenceJSONHandler();
    ~PersistenceJSONHandler();

    nlohmann::json getFavSlots();
    void setFavSlots(nlohmann::json jsonIn);

private:
    nlohmann::json _favSlots;
};