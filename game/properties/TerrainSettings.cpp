//
// Created by tatiana on 27.03.2018.
//

#include "TerrainSettings.h"
#include "TerrainSprites.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <algorithm>

namespace properties {
    TerrainSettings::TerrainSettings()
    {
        mMaterialIDsToRampIDs.push_back({game::MaterialType::Soil,"SoilRamp"});
        mMaterialIDsToRampIDs.push_back({game::MaterialType::Stone,"RawStoneRamp"});
    }

    void TerrainSettings::parseTerrainSettingsFile()
    {
        std::string path="../settings/objects/terrainsettings.xml";

        boost::property_tree::ptree pt;
        boost::property_tree::read_xml(path, pt);

        for (auto const& tree_item : pt.get_child("TerrainSettings"))
        {
            if (tree_item.first=="TerrainSprites")
            {
                std::shared_ptr<TerrainSprites> newTerrainSprite=std::make_shared<TerrainSprites>();
                newTerrainSprite->Key=tree_item.second.get("Key","");

                for (auto const& state_item : tree_item.second.get_child("Value"))
                {
                    if (state_item.first=="StateSprite")
                    {
                        std::string spriteKey=state_item.second.get("Key","");
                        std::string spriteID=state_item.second.get("SpriteID","");
                        if (spriteKey=="Wall")
                            newTerrainSprite->WallID=spriteID;
                        else if (spriteKey=="Floor")
                            newTerrainSprite->FloorID=spriteID;
                        else if (spriteKey=="TopRightRamp")
                            newTerrainSprite->TopRightRamp=spriteID;
                        else if (spriteKey=="TopLeftRamp")
                            newTerrainSprite->TopLeftRamp=spriteID;
                        else if (spriteKey=="BottomRightRamp")
                            newTerrainSprite->BottomRightRamp=spriteID;
                        else if (spriteKey=="BottomLeftRamp")
                            newTerrainSprite->BottomLeftRamp=spriteID;
                    }
                }

                mTerrainSprites.push_back(newTerrainSprite);
            }
        }
    }

    std::shared_ptr<const TerrainSprites> TerrainSettings::terrainSprite(const std::string& materialID) const
    {
        const auto iter=std::find_if(mTerrainSprites.begin(),mTerrainSprites.end(),[&materialID](std::shared_ptr<TerrainSprites> const& item)
        {
            return item->Key==materialID;
        });

        if (iter!=mTerrainSprites.end())
            return (*iter);
        else
            return nullptr;
    }

    std::string TerrainSettings::materialToRampID(const game::MaterialType& materialType) const
    {
        const auto iter=std::find_if(mMaterialIDsToRampIDs.begin(),mMaterialIDsToRampIDs.end(),[&materialType](std::pair<game::MaterialType,std::string> const& item)
        {
            return item.first==materialType;
        });

        if (iter!=mMaterialIDsToRampIDs.end())
            return (*iter).second;

        return "";
    }

    void TerrainSettings::addStoneID(const game::MaterialID_t& materialID)
    {
        const auto iter=std::find_if(StoneMaterialIDs.begin(),StoneMaterialIDs.end(),[&materialID](game::MaterialID_t const& elem)
        {
            return materialID==elem;
        });

        if (iter==StoneMaterialIDs.end())
            StoneMaterialIDs.push_back(materialID);
    }
}