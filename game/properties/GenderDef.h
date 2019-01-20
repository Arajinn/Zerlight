//
// Created by tatiana on 28.12.18.
//

#ifndef ZERLIGHT_GENDERDEF_H
#define ZERLIGHT_GENDERDEF_H

#include "game/core/enums.h"

#include <string>

namespace properties {
    struct GenderDef {
        game::GenderType Gender;
        std::string Name;
        float RandomWeight;
    };
}

#endif //ZERLIGHT_GENDERDEF_H
