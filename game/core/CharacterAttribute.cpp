//
// Created by tatiana on 26.03.2018.
//

#include "CharacterAttribute.h"

namespace game {
    CharacterAttribute::CharacterAttribute()
            :mAttributeLevel(0)
            ,mStartingAttributeLevel(0)
            ,mExperience(0.0f)
    {

    }

    CharacterAttribute::CharacterAttribute(int AttributeLevel)
            :mAttributeLevel(AttributeLevel)
            ,mStartingAttributeLevel(AttributeLevel)
            ,mExperience(0.0f)
    {

    }

    int CharacterAttribute::attributeLevel() const
    {
        return mAttributeLevel;
    }

    int CharacterAttribute::startingAttributeLevel() const
    {
        return mStartingAttributeLevel;
    }

    bool CharacterAttribute::increase(const float& xp)
    {
        mExperience+=xp;
        const float threshold=10.0f+(mAttributeLevel-mStartingAttributeLevel)*5.0f;
        if (mExperience<threshold)
            return false;

        mAttributeLevel++;
        mExperience-=threshold;
        return true;
    }
}