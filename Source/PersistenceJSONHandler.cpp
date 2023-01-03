/*
  ==============================================================================

    persistenceJSONHandler.cpp
    Created: 29 Oct 2022 2:52:54pm
    Author:  Tiparium

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

void PersistenceJSONHandler::addJSONToLocalInstance(juce::String segmentTitle, nlohmann::json jsonIn)
{
    std::string segTitle = segmentTitle.toStdString();
    _localInstance[segTitle] = jsonIn;
}

void PersistenceJSONHandler::saveJSONToFile()
{
    nlohmann::json newFileContents = _localInstance;
    std::ofstream file(_path.toStdString());
    file << newFileContents;
    file.close();
}