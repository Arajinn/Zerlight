//
// Created by tatiana on 27.03.2018.
//

#ifndef ZERL_GAMEDEFINITION_H
#define ZERL_GAMEDEFINITION_H

#include <memory>
#include <vector>

namespace properties
{
    struct TerrainSettings;
    struct RaceDefinition;
    struct ConstructionDef;
    struct ItemDefinition;
    struct GameSettings;
    struct BodyPartDef;
    struct BodyDef;
    struct MaterialDef;
    struct SpriteDef;
}

namespace properties {
    struct GameDefinition {
    public:
        GameDefinition();
        ~GameDefinition();

        const std::shared_ptr<TerrainSettings>& terrainSettings() const;
        int indexOfMaterial(std::string id) const;
        int airMaterialIndex() const;

        std::shared_ptr<const RaceDefinition> raceDefinition(const std::string& id) const;

        std::shared_ptr<const ConstructionDef> constructionDefinition(const std::string& id) const;

        std::shared_ptr<const ItemDefinition> itemDefinition(const std::string& id) const;

        std::shared_ptr<const GameSettings> gameSettings() const;

        std::shared_ptr<const BodyPartDef> bodyPartDefinition(const std::string& id) const;

        std::shared_ptr<const BodyDef> bodyDefinition(const std::string& id) const;

        std::shared_ptr<const MaterialDef> materialDefinition(const std::string& id) const;
        std::shared_ptr<const MaterialDef> materialDefinition(const int& index) const;

        std::shared_ptr<const SpriteDef> spriteDefinition(const std::string& spriteID) const;
    private:
        std::shared_ptr<TerrainSettings> mTerrainSettings;
        std::shared_ptr<GameSettings> mGameSettings;

        std::vector<std::shared_ptr<MaterialDef>> mMaterialDefs;
        int mAirMaterialIndex{-1};
        void parseMaterialDefsFile();

        std::vector<std::shared_ptr<RaceDefinition>> mRaceDefinitions;

        std::vector<std::shared_ptr<ConstructionDef>> mConstructionDefs;

        std::vector<std::shared_ptr<ItemDefinition>> mItemDefinitions;
        void parseItemDefs();

        std::vector<std::shared_ptr<BodyPartDef>> mBodyPartDefs;
        void initBodyPartDefs();
        void parseBodyPartDefs();

        std::vector<std::shared_ptr<BodyDef>> mBodyDefs;
        void initBodyDefs();
        void parseBodyDefs();

        std::vector<std::shared_ptr<SpriteDef>> mSpriteDefs;
        void parseSpriteDefsFile();

        std::vector<std::string> split(std::string data, std::string delimiter);
    };
}

#endif //ZERL_GAMEDEFINITION_H
