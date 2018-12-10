//
// Created by tatiana on 09.06.2018.
//

#include "AIDirector.h"
#include "defines.h"
#include "GameManager.h"
#include "Fortress.h"
#include "Item.h"
#include "Character.h"
#include "game/map/Region.h"
#include "game/map/Map.h"
#include "game/map/MapCell.h"
#include "game/properties/GameDefinition.h"
#include "game/properties/ItemDefinition.h"

namespace game
{
    AIDirector::AIDirector(std::shared_ptr<GameManager> aGameManager)
        :mGameManager(aGameManager)
    {

    }

    AIDirector::~AIDirector()
    {

    }

    std::shared_ptr<Item> AIDirector::generateItem(const map::vector3& position, std::string itemID, std::string materialID, ItemQuality quality)
    {
        std::shared_ptr<Item> craftableItem=Item::create(position,itemID,materialID,GAME_DEFINITIONS->itemDefinition(itemID));
        return craftableItem;
    }

    std::shared_ptr<Item> AIDirector::createItem(const map::vector3& position, std::string itemID, std::string materialID)
    {
        auto object=this->generateItem(position,itemID,materialID);
        mGameManager->addToSpawnList(object);

        auto fortress=mGameManager->region()->fortress();
        fortress->addItem(object);

        return object;
    }

    map::vector3 AIDirector::findGroundPos(const map::vector3& pos)
    {
        auto map=mGameManager->region()->map();
        map::vector3 find_pos=map::vector3(pos.x(),pos.y(),map->mapDepth()-1);
        bool stop=false;
        while (!stop)
        {
            auto cell=map->cell(find_pos);

            if (cell== nullptr)
                find_pos.set_z(find_pos.z()+1);

            if (!cell->hasNaturalFloor())
                find_pos.set_z(find_pos.z()-1);
            else
                stop=true;
        }

        return find_pos;
    }

    bool AIDirector::isPlayerFactionAutomatonRace(std::string raceID)
    {
        return false;
    }

    void AIDirector::createAutomaton(map::vector3 embarkPos, properties::SettlerSettings settlerSettings)
    {

    }

    void AIDirector::createSettler(map::vector3 embarkPos, properties::SettlerSettings settlerSettings)
    {
        auto groundPos=this->findGroundPos(embarkPos+settlerSettings.offset);
        if (this->isPlayerFactionAutomatonRace(settlerSettings.RaceID))
        {
            return this->createAutomaton(embarkPos,settlerSettings);
        }
        else
        {
            std::shared_ptr<Character> settler=Character::create(groundPos,GAME_DEFINITIONS->raceDefinition(settlerSettings.RaceID));
            settler->setBehavior(behavior::BehaviorType::PlayerCharacter);

            mGameManager->addToSpawnList(settler);
        }
    }
}