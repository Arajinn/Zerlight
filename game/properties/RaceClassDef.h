//
// Created by tatiana on 23.12.18.
//

#ifndef ZERLIGHT_RACECLASSDEF_H
#define ZERLIGHT_RACECLASSDEF_H

#include "game/core/enums.h"

#include <vector>
#include <string>

namespace properties {
    struct RaceClassDef {
        std::string RaceID;
        std::string Name;
        float BaseCombatValue;
        std::vector<std::string> TemplateMaterialIDs;
    };
}

#endif //ZERLIGHT_RACECLASSDEF_H
