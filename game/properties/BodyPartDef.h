//
// Created by tatiana.polozova on 6/14/2018.
//

#ifndef ZERL_BODYPARTDEF_H
#define ZERL_BODYPARTDEF_H

#include "game/core/enums.h"

#include <string>
#include <vector>
#include <memory>

namespace properties {
    struct BodyPartDef {
        BodyPartDef();
        ~BodyPartDef();

        std::string ID;
        std::string Name;
        game::Material MaterialID;
        std::vector<game::BodyPartProperty> BodyProperties;
        bool Symmetrical;
        std::vector<std::shared_ptr<BodyPartDef>> ContainedParts;
        game::BodyFunction BodyFunction;

        float Thickness;
        float ToHitWeight;

        std::string HarvestedItem;
        int HarvestedQuantity;
    };
}

#endif //ZERL_BODYPARTDEF_H
