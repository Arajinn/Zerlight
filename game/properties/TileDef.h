//
// Created by tatiana on 15.12.18.
//

#ifndef ZERLIGHT_TILEDEF_H
#define ZERLIGHT_TILEDEF_H

#include "guichan/gui/ZColor.h"

namespace properties
{
    struct TileDef
    {
        std::string SpriteID;
        float DrawOrder;
        gui::ZColor Color;
    };
}

#endif //ZERLIGHT_TILEDEF_H
