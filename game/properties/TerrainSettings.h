//
// Created by tatiana on 27.03.2018.
//

#ifndef ZERL_TERRAINSETTINGS_H
#define ZERL_TERRAINSETTINGS_H

#include "game/core/enums.h"
#include "game/core/types.h"
#include <string>
#include <vector>
#include <memory>

namespace properties {
    struct TerrainSprites;

    struct TerrainSettings
    {
    public:
        TerrainSettings();
        game::MaterialID_t AirMaterialID;
        game::MaterialID_t DirtMaterialID;
        game::MaterialID_t ClayMaterialID;
        game::MaterialID_t WaterMaterialID;
        game::MaterialID_t LavaMaterialID;
        game::MaterialID_t GrownOnMaterialID;
        std::vector<game::MaterialID_t> StoneMaterialIDs;

        void parseTerrainSettingsFile();

        std::shared_ptr<const TerrainSprites> terrainSprite(const std::string& materialID) const;

        std::string materialToRampID(const game::MaterialType& materialType) const;

        void addStoneID(const game::MaterialID_t& materialID);
    private:
        std::vector<std::shared_ptr<TerrainSprites>> mTerrainSprites;
        std::vector<std::pair<game::MaterialType,std::string>> mMaterialIDsToRampIDs;
    };
}

#endif //ZERL_TERRAINSETTINGS_H
