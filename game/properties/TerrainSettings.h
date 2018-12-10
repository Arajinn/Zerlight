//
// Created by tatiana on 27.03.2018.
//

#ifndef ZERL_TERRAINSETTINGS_H
#define ZERL_TERRAINSETTINGS_H

#include "game/core/enums.h"
#include <string>
#include <vector>
#include <memory>

namespace properties {
    struct TerrainSprites;

    struct TerrainSettings
    {
    public:
        TerrainSettings();
        std::string AirMaterialID;
        std::string LavaMaterialID;

        void parseTerrainSettingsFile();

        std::shared_ptr<const TerrainSprites> terrainSprite(const std::string& materialID) const;

        std::string materialToRampID(const game::MaterialType& materialType) const;
    private:
        std::vector<std::shared_ptr<TerrainSprites>> mTerrainSprites;
        std::vector<std::pair<game::MaterialType,std::string>> mMaterialIDsToRampIDs;
    };
}

#endif //ZERL_TERRAINSETTINGS_H
