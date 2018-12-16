//
// Created by tatiana on 09.06.2018.
//

#include "CharacterHistory.h"
#include "game/properties/RaceDefinition.h"

namespace game
{
    CharacterHistory::CharacterHistory(std::shared_ptr<const properties::RaceDefinition> aRaceDef)
        :raceDef(aRaceDef)
    {
        mGender=aRaceDef->generateGender();
    }

    CharacterHistory::~CharacterHistory()
    {

    }

    void CharacterHistory::setName(const std::string& value)
    {
        mName=value;
    }

    const GenderType& CharacterHistory::gender() const
    {
        return mGender;
    }
}