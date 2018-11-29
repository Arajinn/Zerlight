//
// Created by tatiana.polozova on 25.06.2018.
//

#ifndef ZERL_MATERIALDEF_H
#define ZERL_MATERIALDEF_H

#include "game/core/enums.h"
#include "guichan/gui/ZColor.h"

#include <string>

namespace properties
{
    struct MaterialDef
    {
    public:
        std::string ID;
        std::string Name;

        game::MaterialType Type;

        float Strength;
        float Value;

        gui::ZColor Color;
    };
}

#endif //ZERL_MATERIALDEF_H
