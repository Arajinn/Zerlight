//
// Created by tatiana.polozova on 27.03.2018.
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
}

namespace properties {
    struct GameDefinition {
    public:
        GameDefinition();
        ~GameDefinition();

        const std::shared_ptr<TerrainSettings>& terrainSettings() const;
        int indexOfMaterial(std::string id) const;

        const std::shared_ptr<RaceDefinition>& raceDefinition(const std::string& id) const;

        const std::shared_ptr<ConstructionDef>& constructionDefinition(const std::string& id) const;

        const std::shared_ptr<ItemDefinition>& itemDefinition(const std::string& id) const;

        const std::shared_ptr<GameSettings>& gameSettings() const;

        const std::shared_ptr<BodyPartDef>& bodyPartDefinition(const std::string& id) const;

        const std::shared_ptr<BodyDef>& bodyDefinition(const std::string& id) const;

        const std::shared_ptr<MaterialDef>& materialDefinition(const std::string& id) const;
    private:
        std::shared_ptr<TerrainSettings> mTerrainSettings;
        std::shared_ptr<GameSettings> mGameSettings;

        std::vector<std::shared_ptr<MaterialDef>> mMaterials;

        std::vector<std::shared_ptr<RaceDefinition>> mRaceDefinitions;

        std::vector<std::shared_ptr<ConstructionDef>> mConstructionDefs;

        std::vector<std::shared_ptr<ItemDefinition>> mItemDefinitions;

        std::vector<std::shared_ptr<BodyPartDef>> mBodyPartDefs;
        void initBodyPartDefs();

        std::vector<std::shared_ptr<BodyDef>> mBodyDefs;
        void initBodyDefs();
    };
}

#endif //ZERL_GAMEDEFINITION_H
