//
// Created by tatiana on 04.04.2018.
//

#ifndef ZERL_ATTRIBUTEDEF_H
#define ZERL_ATTRIBUTEDEF_H

#include "game/core/enums.h"

namespace properties {
    struct AttributeDef {
        AttributeDef();
        AttributeDef(game::CharacterAttributeType Attribute,int Min,int Max,int Mean);
        game::CharacterAttributeType Attribute;
        int Min;
        int Max;
        int Mean;
    };
}

#endif //ZERL_ATTRIBUTEDEF_H
