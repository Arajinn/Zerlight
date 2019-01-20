//
// Created by tatiana on 29.12.18.
//

#ifndef ZERLIGHT_NATURALWEAPONDEF_H
#define ZERLIGHT_NATURALWEAPONDEF_H

#include "game/core/enums.h"

#include <string>
#include <memory>

namespace properties
{
    struct WeaponDef;
}

namespace properties {
    struct NaturalWeaponDef {
        std::string Name;
        float Size;
        int TemplateMaterialIndex;
        std::string MaterialID;
        std::shared_ptr<WeaponDef> weaponDef{nullptr};
    };
}

#endif //ZERLIGHT_NATURALWEAPONDEF_H
