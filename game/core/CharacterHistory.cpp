//
// Created by tatiana.polozova on 09.06.2018.
//

#include "CharacterHistory.h"

namespace game
{
    CharacterHistory::CharacterHistory(std::shared_ptr<properties::RaceDefinition> aRaceDef)
        :raceDef(aRaceDef)
    {

    }

    CharacterHistory::~CharacterHistory()
    {

    }

    void CharacterHistory::setName(const std::string& value)
    {
        mName=value;
    }
}