/*
  ==============================================================================

    persistenceJSONHandler.cpp
    Created: 29 Oct 2022 2:52:54pm
    Author:  merje

  ==============================================================================
*/

#include "persistenceJSONHandler.h"

PersistenceJSONHandler::PersistenceJSONHandler(juce::String path)
    : _path(path)
{
}
PersistenceJSONHandler::~PersistenceJSONHandler()
{
}

nlohmann::json PersistenceJSONHandler::readJSONFromFile()
{
    std::ifstream file(_path.toStdString());
    nlohmann::json temp = nlohmann::json::parse(file);
    file.close();
    return temp;
}

void PersistenceJSONHandler::saveJSONToFile(nlohmann::json jsonIn)
{
    nlohmann::json newFileContents = jsonIn;
    std::ofstream file(_path.toStdString());
    file << newFileContents;
    file.close();
}