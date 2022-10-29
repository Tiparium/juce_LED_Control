/*
  ==============================================================================

    persistenceJSONHandler.cpp
    Created: 29 Oct 2022 2:52:54pm
    Author:  merje

  ==============================================================================
*/

#include "persistenceJSONHandler.h"

PersistenceJSONHandler::PersistenceJSONHandler()
{
    std::ifstream f("../../resources/favSlots.json");
    _favSlots = nlohmann::json::parse(f);
    DBG(_favSlots.dump());
}
PersistenceJSONHandler::~PersistenceJSONHandler() {}

nlohmann::json PersistenceJSONHandler::getFavSlots()
{
    return _favSlots;
}

void PersistenceJSONHandler::setFavSlots(nlohmann::json jsonIn)
{

}