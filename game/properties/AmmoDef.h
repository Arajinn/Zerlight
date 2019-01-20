//
// Created by tatiana on 26.12.18.
//

#ifndef ZERLIGHT_AMMODEF_H
#define ZERLIGHT_AMMODEF_H

#include "game/core/enums.h"

#include <vector>
#include <string>

namespace properties {
    struct AmmoDef {
        std::string AmmoID;
        std::vector<game::DamageType> DamageTypes;
        float WeaponSize;
        float VelocityModifier;
        float BluntModifier;
        float Point;
        float Edge;
    };
}

#endif //ZERLIGHT_AMMODEF_H
