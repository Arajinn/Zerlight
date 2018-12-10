//
// Created by tatiana on 09.06.2018.
//

#include "CharacterHistory.h"

namespace game
{
    CharacterHistory::CharacterHistory(std::shared_ptr<const properties::RaceDefinition> aRaceDef)
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