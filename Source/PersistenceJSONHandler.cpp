/*
  ==============================================================================

    persistenceJSONHandler.cpp
    Created: 29 Oct 2022 2:52:54pm
    Author:  Tiparium

    // Backup of my favorite settings:
    {"favorites":[{"b":0,"g":0,"r":255},{"b":0,"g":255,"r":0},{"b":255,"g":0,"r":0},{"b":174,"g":242,"r":255},{"b":201,"g":255,"r":255}]}
  ==============================================================================
*/

#include "persistenceJSONHandler.h"

PersistenceJSONHandler::PersistenceJSONHandler(juce::String path)
    : _path(path)
{
    checkForEmptyJSON();
}
PersistenceJSONHandler::~PersistenceJSONHandler()
{
}

void PersistenceJSONHandler::checkForEmptyJSON()
{
    try
    {
        readJSONFromFile();
    }
    catch (nlohmann::json_abi_v3_11_2::detail::parse_error)
    {
        DBG("First Run or Empty JSON detected, rebuilding");
        nlohmann::json newInstance;
        nlohmann::json emptyArr = nlohmann::json::array();
        newInstance["favorites"] = emptyArr;
        newInstance["phue_led_bools"] = emptyArr;

        nlohmann::json emptyRGB;
        emptyRGB["r"] = 0;
        emptyRGB["g"] = 0;
        emptyRGB["b"] = 0;

        newInstance["base_color_state"] = emptyRGB;
        _localInstance = newInstance;
        saveJSONToFile();
    }

}

nlohmann::json PersistenceJSONHandler::readJSONFromFile()
{
    std::ifstream file(_path.toStdString());
    nlohmann::json temp = nlohmann::json::parse(file);
    file.close();
    return temp;
}

void PersistenceJSONHandler::addJSONToLocalInstance(juce::String segmentTitle, nlohmann::json jsonIn)
{
    _localInstance[segmentTitle.toStdString()] = jsonIn;
}

void PersistenceJSONHandler::saveJSONToFile()
{
    nlohmann::json newFileContents = _localInstance;
    std::string jsonAsStr = newFileContents.dump(4);
    std::ofstream file(_path.toStdString());
    // file << newFileContents;                     // For Ugly file save
    file << std::setw(4) << jsonAsStr << std::endl; // For Pretty file save
    file.close();
}