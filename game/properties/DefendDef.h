//
// Created by tatiana on 28.12.18.
//

#ifndef ZERLIGHT_DEFENDDEF_H
#define ZERLIGHT_DEFENDDEF_H

#include "game/core/enums.h"

#include <vector>
#include <string>

namespace properties {
    struct DefendDef {
        float DefendTime;
        float AttackDelay;
        std::vector<game::AttackType> DefendTypes;
        std::string Verb;

        bool defendsAgainst(const game::AttackType& type) const;
    };
}

#endif //ZERLIGHT_DEFENDDEF_H
