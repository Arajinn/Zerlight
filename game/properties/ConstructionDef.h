//
// Created by tatiana on 2018-04-20.
//

#ifndef ZERL_CONSTRUCTIONDEF_H
#define ZERL_CONSTRUCTIONDEF_H

#include "game/core/enums.h"

#include <string>
#include <vector>

namespace properties {
    struct ConstructionDef
    {
        ConstructionDef();
        ~ConstructionDef();

        std::string ID;
        std::string Name;
        std::string Prefix;

        std::vector<game::ConstructionProperty> Properties;

        bool hasFlag(game::ConstructionProperty flag) const;
    };
}

#endif //ZERL_CONSTRUCTIONDEF_H
