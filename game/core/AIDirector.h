//
// Created by tatiana on 09.06.2018.
//

#ifndef ZERL_AIDIRECTOR_H
#define ZERL_AIDIRECTOR_H

#include "game/core/enums.h"
#include "game/core/types.h"
#include "game/properties/SettlerSettings.h"

#include <memory>
#include <vector>

namespace game
{
    class Item;
    class Character;
    class BodySection;
    class Faction;
    class CreateWorldOptions;
    class GameManager;
}

namespace game {
    class AIDirector {
    public:
        AIDirector(std::shared_ptr<GameManager> aGameManager);
        ~AIDirector();

        std::shared_ptr<Item> createItem(const map::vector3& position, const std::string& itemID, const MaterialID_t& materialID);
        void createBodyPartItem(const map::vector3& position, std::shared_ptr<Character> character, std::shared_ptr<BodySection> limb,
                                const std::string& itemID);
        void createSettler(map::vector3 embarkPos, properties::SettlerSettings settlerSettings);
        void createAutomaton(map::vector3 embarkPos, properties::SettlerSettings settlerSettings);

        void createWildAnimal(map::vector3 embarkPos, const map::vector3& offset, const std::string& raceID);

        void generateStartingFactions(std::shared_ptr<const CreateWorldOptions> worldOptions);

        std::shared_ptr<Faction> faction(const size_t& factionID) const;
        std::vector<std::shared_ptr<Faction>> factions() const;

        size_t playerFactionID() const;
    private:
        std::shared_ptr<GameManager> mGameManager;

        std::shared_ptr<Item> generateItem(const map::vector3& position, const std::string& itemID, const MaterialID_t& materialID, ItemQuality quality = ItemQuality::Average);
        void postCreateItem(std::shared_ptr<Item> item);

        map::vector3 findGroundPos(const map::vector3& pos);

        bool isPlayerFactionAutomatonRace(std::string raceID);

        std::vector<std::shared_ptr<Faction>> mFactions;
        size_t mPlayerFactionID;
        void generatePlayerFaction(const std::string& name);
        void generateWildFaction();
    };
}

#endif //ZERL_AIDIRECTOR_H
