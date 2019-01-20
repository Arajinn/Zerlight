//
// Created by tatiana on 6/14/2018.
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

        std::string ID;
        std::string Name;
        std::string BodyPartID;
        std::string MaterialID;
        game::EquipmentType EquipType;

        float ToHitWeight;

        bool Limb{false};
        bool Connection{false};
        bool Symmetrical{false};
        bool Hand{false};

        std::vector<std::shared_ptr<BodySectionDef>> ConnectedSections;

        std::string SpriteID;
        std::string SpriteIDRight;
        std::string SpriteIDLeft;
        int DrawOrder;
        std::vector<std::string> Decorations,DecorationsRight,DecorationsLeft;
        std::vector<std::vector<std::string>> AdditionalDecoration,AdditionalDecorationMale,AdditionalDecorationFemale;

        int TemplateMaterialIndex;
    };
}

#endif //ZERL_BODYSECTIONDEF_H
