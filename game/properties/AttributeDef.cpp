//
// Created by tatiana.polozova on 04.04.2018.
//

#include "AttributeDef.h"

namespace properties {
    AttributeDef::AttributeDef()
            :Attribute(game::CharacterAttributeType::Undef)
            ,Min(0)
            ,Max(0)
            ,Mean(0)
    {

    }

    AttributeDef::AttributeDef(game::CharacterAttributeType Attribute,int Min,int Max,int Mean)
            :Attribute(Attribute)
            ,Min(Min)
            ,Max(Max)
            ,Mean(Mean)
    {

    }
}