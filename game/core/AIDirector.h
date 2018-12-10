//
// Created by tatiana on 09.06.2018.
//

#ifndef ZERL_AIDIRECTOR_H
#define ZERL_AIDIRECTOR_H

#include "game/core/enums.h"
#include "game/properties/SettlerSettings.h"

#include <memory>

namespace game
{
    class Item;
    class GameManager;
}

namespace game {
    class AIDirector {
    public:
        AIDirector(std::shared_ptr<GameManager> aGameManager);
        ~AIDirector();

        std::shared_ptr<Item> createItem(const map::vector3& position, std::string itemID, std::string materialID);
        void createSettler(map::vector3 embarkPos, properties::SettlerSettings settlerSettings);
        void createAutomaton(map::vector3 embarkPos, properties::SettlerSettings settlerSettings);
    private:
        std::shared_ptr<GameManager> mGameManager;

        std::shared_ptr<Item> generateItem(const map::vector3& position, std::string itemID, std::string materialID, ItemQuality quality = ItemQuality::Average);

        map::vector3 findGroundPos(const map::vector3& pos);

        bool isPlayerFactionAutomatonRace(std::string raceID);
    };
}

#endif //ZERL_AIDIRECTOR_H
