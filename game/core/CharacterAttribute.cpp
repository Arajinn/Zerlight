//
// Created by tatiana.polozova on 26.03.2018.
//

#include "CharacterAttribute.h"

namespace game {
    CharacterAttribute::CharacterAttribute()
            :AttributeLevel(0)
            ,StartingAttributeLevel(0)
            ,Experience(0.0f)
    {

    }

    CharacterAttribute::CharacterAttribute(int AttributeLevel)
            :AttributeLevel(AttributeLevel)
            ,StartingAttributeLevel(AttributeLevel)
            ,Experience(0.0f)
    {

    }
}