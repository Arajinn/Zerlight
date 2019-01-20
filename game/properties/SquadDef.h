//
// Created by tatiana on 07.01.19.
//

#ifndef ZERLIGHT_SQUADDEF_H
#define ZERLIGHT_SQUADDEF_H

#include "game/core/enums.h"

#include <vector>
#include <memory>

namespace properties
{
    struct RaceClassDef;
}

namespace properties {
    struct SquadDef {
        float BaseCombatValue;
        game::SquadType Type;
        bool AllClasses;
        std::vector<std::shared_ptr<RaceClassDef>> Classes;
    };
}

#endif //ZERLIGHT_SQUADDEF_H
