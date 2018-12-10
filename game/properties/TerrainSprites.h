//
// Created by tatiana on 07.12.18.
//

#ifndef ZERLIGHT_TERRAINSPRITES_H
#define ZERLIGHT_TERRAINSPRITES_H

#include <string>

namespace properties {
    struct TerrainSprites
    {
        std::string Key;
        std::string WallID;
        std::string FloorID;

        std::string TopRightRamp, TopLeftRamp, BottomRightRamp, BottomLeftRamp;
    };
}

#endif //ZERLIGHT_TERRAINSPRITES_H
