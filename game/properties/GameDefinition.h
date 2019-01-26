//
// Created by tatiana on 27.03.2018.
//

#ifndef ZERL_GAMEDEFINITION_H
#define ZERL_GAMEDEFINITION_H

#include "game/core/types.h"
#include "game/properties/FactionDef.h"

#include <memory>
#include <vector>

namespace game
{
    class Item;
}

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
    struct FactionDef;
    struct ItemSettings;
    struct UniformSettings;
    struct AmmoDef;
    struct CharacterSettings;
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

        std::shared_ptr<const MaterialDef> materialDefinition(const game::MaterialID_t& id) const;
        std::shared_ptr<const MaterialDef> materialDefinition(const int& index) const;

        std::shared_ptr<const SpriteDef> spriteDefinition(const std::string& spriteID) const;

        int playerFactionIndex() const;
        std::shared_ptr<const FactionDef> factionDef(const int& index) const;
        std::shared_ptr<const FactionDef> factionDef(const std::string& factionID) const;
        std::vector<std::shared_ptr<const FactionDef>> factionDefs(const game::FactionType& type) const;

        std::shared_ptr<const ItemSettings> itemSettings() const;

        std::shared_ptr<const UniformSettings> uniformSettings() const;

        std::shared_ptr<const AmmoDef> ammoDefinition(const std::string& id) const;

        static std::string itemName(const std::string& itemID,const std::string& materialID,std::shared_ptr<const game::Item> item=nullptr);

        std::shared_ptr<const CharacterSettings> characterSettings() const;
    private:
        std::shared_ptr<TerrainSettings> mTerrainSettings;
        std::shared_ptr<GameSettings> mGameSettings;

        std::vector<std::shared_ptr<MaterialDef>> mMaterialDefs;
        int mAirMaterialIndex{-1};
        void parseMaterialDefs();

        std::vector<std::shared_ptr<RaceDefinition>> mRaceDefinitions;
        void parseRaceDefs();

        std::vector<std::shared_ptr<ConstructionDef>> mConstructionDefs;

        std::vector<std::shared_ptr<ItemDefinition>> mItemDefinitions;
        void parseItemDefs();

        std::vector<std::shared_ptr<BodyPartDef>> mBodyPartDefs;
        void parseBodyPartDefs();

        std::vector<std::shared_ptr<BodyDef>> mBodyDefs;
        void parseBodyDefs();

        std::vector<std::shared_ptr<SpriteDef>> mSpriteDefs;
        void parseSpriteDefsFile();

        std::vector<std::string> split(std::string data, std::string delimiter);

        std::vector<std::shared_ptr<FactionDef>> mFactionDefs;
        int mPlayerFactionIndex;
        void parseFactionDefs();

        std::shared_ptr<ItemSettings> mItemSettings;
        std::shared_ptr<UniformSettings> mUniformSettings;

        std::vector<std::shared_ptr<AmmoDef>> mAmmoDefs;

        std::shared_ptr<CharacterSettings> mCharacterSettings;
    };
}

#endif //ZERL_GAMEDEFINITION_H
