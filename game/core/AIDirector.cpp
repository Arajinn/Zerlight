//
// Created by tatiana on 09.06.2018.
//

#include "AIDirector.h"
#include "defines.h"
#include "GameManager.h"
#include "Fortress.h"
#include "Item.h"
#include "BodyPartItem.h"
#include "Character.h"
#include "Faction.h"
#include "CreateWorldOptions.h"
#include "game/map/Region.h"
#include "game/map/Map.h"
#include "game/map/MapCell.h"
#include "game/properties/GameDefinition.h"
#include "game/properties/ItemDefinition.h"
#include "game/utils/Randomizer.h"

namespace game
{
    AIDirector::AIDirector(std::shared_ptr<GameManager> aGameManager)
        :mGameManager(aGameManager)
    {
        mPlayerFactionID=0;
    }

    AIDirector::~AIDirector()
    {

    }

    std::shared_ptr<Item> AIDirector::generateItem(const map::vector3& position, std::string itemID, std::string materialID,
            ItemQuality quality)
    {
        std::shared_ptr<Item> craftableItem=Item::create(position,itemID,materialID,GAME_DEFINITIONS->itemDefinition(itemID));
        return craftableItem;
    }

    std::shared_ptr<Item> AIDirector::createItem(const map::vector3& position, const std::string& itemID, const std::string& materialID)
    {
        auto object=this->generateItem(position,itemID,materialID);
        postCreateItem(object);
        return object;
    }

    void AIDirector::createBodyPartItem(const map::vector3& position, std::shared_ptr<Character> character,
            std::shared_ptr<BodySection> limb, const std::string& itemID)
    {
        auto object=BodyPartItem::create(position,character,limb,itemID,GAME_DEFINITIONS->itemDefinition(itemID));
        postCreateItem(object);
    }

    void AIDirector::postCreateItem(std::shared_ptr<Item> item)
    {
        mGameManager->addToSpawnList(item);

        auto fortress=mGameManager->region()->fortress();
        fortress->addItem(item);
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
        const auto groundPos=this->findGroundPos(embarkPos+settlerSettings.offset);
        if (this->isPlayerFactionAutomatonRace(settlerSettings.RaceID))
        {
            return this->createAutomaton(embarkPos,settlerSettings);
        }
        else
        {
            const auto raceDef=GAME_DEFINITIONS->raceDefinition(settlerSettings.RaceID);

            const auto factionList=GAME_DEFINITIONS->factionDefs(FactionType::PlayerCiv);
            if (factionList.empty())
                return;

            const auto rand=(size_t)RANDOMIZER->rand(0,(int)factionList.size()-1);
            const auto factionDef=factionList.at(rand);

            std::shared_ptr<Character> settler=Character::create(groundPos,raceDef,factionDef,mPlayerFactionID);
            settler->setBehavior(behavior::BehaviorType::PlayerCharacter);

            AIDIRECTOR->faction(mPlayerFactionID)->addMember(settler);

            mGameManager->addToSpawnList(settler);
        }
    }

    void AIDirector::createWildAnimal(map::vector3 embarkPos, const map::vector3& offset, const std::string& raceID)
    {
        const auto groundPos=this->findGroundPos(embarkPos+offset);

        const auto raceDef=GAME_DEFINITIONS->raceDefinition(raceID);

        const auto factionList=GAME_DEFINITIONS->factionDefs(FactionType::Wild);
        if (factionList.empty())
            return;

        const int rand=RANDOMIZER->rand(0,(int)factionList.size()-1);
        const auto factionDef=factionList.at((size_t)rand);

        const auto factionID=mFactions.size();
        const auto faction=std::make_shared<Faction>(factionDef,"Wild",factionID,6.0f);
        faction->setFactionRelation(mPlayerFactionID,-1.0f);
        mFactions.push_back(faction);

        std::shared_ptr<Character> character=Character::create(groundPos,raceDef,factionDef,factionID);
        character->setBehavior(behavior::BehaviorType::WildAnimal);
        faction->addMember(character);

        mGameManager->addToSpawnList(character);
    }


    std::shared_ptr<Faction> AIDirector::faction(const size_t& factionID) const
    {
        return mFactions.at(factionID);
    }

    std::vector<std::shared_ptr<Faction>> AIDirector::factions() const
    {
        return mFactions;
    }

    void AIDirector::generatePlayerFaction(const std::string& name)
    {

        const auto factionDefs=GAME_DEFINITIONS->factionDefs(FactionType::PlayerCiv);
        if (factionDefs.empty())
            return;

        const auto rand=(size_t)RANDOMIZER->rand(0,(int)factionDefs.size()-1);
        const auto factionID=mFactions.size();
        const auto faction=std::make_shared<Faction>(factionDefs.at(rand),name,factionID,6.0f);
        mFactions.push_back(faction);
        mPlayerFactionID=factionID;
    }

    void AIDirector::generateWildFaction()
    {
        const auto factionDefs=GAME_DEFINITIONS->factionDefs(FactionType::Wild);
        if (factionDefs.empty())
            return;

        const auto rand=(size_t)RANDOMIZER->rand(0,(int)factionDefs.size()-1);
        const auto factionID=mFactions.size();
        const auto faction=std::make_shared<Faction>(factionDefs.at(rand),"Wild",factionID,6.0f);
        faction->setFactionRelation(mPlayerFactionID,-1.0f);
        mFactions.push_back(faction);
    }

    void AIDirector::generateStartingFactions(std::shared_ptr<const CreateWorldOptions> worldOptions)
    {
        mFactions.clear();
        generatePlayerFaction(worldOptions->KingdomName);
        //generateWildFaction();
    }

    size_t AIDirector::playerFactionID() const
    {
        return mPlayerFactionID;
    }
}