//
// Created by tatiana on 26.03.2018.
//

#ifndef ZERL_CHARACTERATTRIBUTE_H
#define ZERL_CHARACTERATTRIBUTE_H

namespace game {
    struct CharacterAttribute {
        CharacterAttribute();
        CharacterAttribute(int AttributeLevel);
        int AttributeLevel;
        int StartingAttributeLevel;
        float Experience;
    };
}

#endif //ZERL_CHARACTERATTRIBUTE_H
