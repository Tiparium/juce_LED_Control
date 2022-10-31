/*
  ==============================================================================

    persistenceJSONHandler.cpp
    Created: 29 Oct 2022 2:52:54pm
    Author:  merje

  ==============================================================================
*/

#include "persistenceJSONHandler.h"

PersistenceJSONHandler::PersistenceJSONHandler(juce::String path, juce::String rootName)
{
    _rootName = rootName;
    _path = path;
}
PersistenceJSONHandler::~PersistenceJSONHandler()
{
    _rootName = getRootName();
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
    std::ofstream file(_path.toStdString());
    file << jsonIn;
    file.close();
}

juce::String PersistenceJSONHandler::getRootName()
{
    return _rootName;
}