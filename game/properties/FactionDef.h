//
// Created by tatiana on 23.12.18.
//

#ifndef ZERLIGHT_FACTIONDEF_H
#define ZERLIGHT_FACTIONDEF_H

#include "game/core/enums.h"

#include <string>
#include <vector>
#include <memory>

namespace properties
{
    struct SquadDef;
}

namespace properties {
    struct FactionDef {
        std::string ID;
        std::string SubType;
        std::string Description;
        game::FactionType Type;
        std::string LanguageID;

        std::vector<std::shared_ptr<SquadDef>> Squads;
    };
}

#endif //ZERLIGHT_FACTIONDEF_H
