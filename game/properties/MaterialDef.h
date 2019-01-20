//
// Created by tatiana on 25.06.2018.
//

#ifndef ZERL_MATERIALDEF_H
#define ZERL_MATERIALDEF_H

#include "game/core/enums.h"
#include "guichan/gui/ZColor.h"

#include <string>
#include <vector>
#include <memory>

namespace properties
{
    struct DamageDef;
}

namespace properties
{
    struct MaterialDef
    {
    public:
        MaterialDef();
        ~MaterialDef();

        std::string ID;
        std::string Name;
        std::string GroupName;
        bool OnlyAddUniqueName;

        game::MaterialType Type;

        float Strength;
        float Value;
        float PierceModifier;
        float SlashModifier;
        float BluntModifier;
        float Sustains;

        gui::ZColor Color;

        std::vector<std::shared_ptr<DamageDef>> DamageProperties;

        std::string GroupNameOrName() const;
    };
}

#endif //ZERL_MATERIALDEF_H
