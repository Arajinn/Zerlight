//
// Created by tatiana.polozova on 6/14/2018.
//

#ifndef ZERL_BODYSECTIONDEF_H
#define ZERL_BODYSECTIONDEF_H

#include "game/core/enums.h"

#include <string>
#include <vector>
#include <memory>

namespace properties {
    struct BodySectionDef {
    public:
        BodySectionDef();
        ~BodySectionDef();

        std::string Name;
        std::string BodyPartID;
        game::Material MaterialID;
        game::EquipmentType EquipType;

        float ToHitWeight;

        bool Limb;
        bool Connection;
        bool Symmetrical;
        bool Hand;

        std::vector<std::shared_ptr<BodySectionDef>> ConnectedSections;
    };
}

#endif //ZERL_BODYSECTIONDEF_H
