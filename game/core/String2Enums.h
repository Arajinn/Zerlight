//
// Created by tatiana on 07.12.18.
//

#ifndef ZERLIGHT_STRING2ENUMS_H
#define ZERLIGHT_STRING2ENUMS_H

#include "game/core/enums.h"

#include <string>

namespace game {
    class String2Enums
    {
    public:
        static MaterialType str2MaterialType(const std::string& str);
        static ItemEffectType str2ItemEffectType(const std::string& str);
        static BodyPartProperty str2BodyPartProperty(const std::string& str);
        static BodyFunction str2BodyFunction(const std::string& str);
        static EquipmentType str2EquipmentType(const std::string& str);
        static AttackType str2AttackType(const std::string& str);
        static DamageType str2DamageType(const std::string& str);
        static GenderType str2GenderType(const std::string& str);
        static CharacterAttributeType str2CharacterAttributeType(const std::string& str);
        static FactionType str2FactionType(const std::string& str);
        static SquadType str2SquadType(const std::string& str);
    };
}

#endif //ZERLIGHT_STRING2ENUMS_H
