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
        static Material str2Material(const std::string& str);
    };
}

#endif //ZERLIGHT_STRING2ENUMS_H
