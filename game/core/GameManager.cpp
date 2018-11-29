//
// Created by tatiana.polozova on 26.03.2018.
//

#include "GameManager.h"
#include "Character.h"
#include "Construction.h"
#include "Item.h"
#include "AIDirector.h"
#include "Stairs.h"
#include "game/map/Region.h"
#include "game/map/Map.h"
#include "game/map/MapCell.h"
#include "game/properties/GameDefinition.h"
#include "game/behavior/TaskResult.h"

#include <algorithm>

namespace game
{
    GameManager::GameManager()
    {

    }

    GameManager::~GameManager()
    {

    }

    std::shared_ptr<GameManager> GameManager::create()
    {
        std::shared_ptr<GameManager> ptr=std::shared_ptr<GameManager>(new GameManager());
        ptr->init();
        return ptr;
    }

    void GameManager::init()
    {
        mNextGameObjectID=0;
        mNextNodeID=0;
        mNextNavGraphID=0;

        mRegion=std::shared_ptr<map::Region>(new map::Region());
        mGameDefs=std::shared_ptr<properties::GameDefinition>(new properties::GameDefinition());
        mAIDirector=std::shared_ptr<AIDirector>(new AIDirector(shared_from_this()));
    }

    unsigned int GameManager::nextNavNodeID()
    {
        mNextNodeID++;
        return mNextNodeID;
    }

    unsigned int GameManager::nextNavID()
    {
        mNextNavGraphID++;
        return mNextNavGraphID;
    }

    void GameManager::testSpawn()
    {
        map::vector3 groundPosStair(20,20,20);
        auto entStair=Stairs::create(groundPosStair,"SoilStairs");
        this->addToSpawnList(entStair);

        map::vector3 groundPosSettler(56,43,20);
        properties::SettlerSettings settlerSettings;
        settlerSettings.RaceID="Gnome";
        settlerSettings.offset=map::vector3(0,0,0);
        settlerSettings.Profession="";
        mAIDirector->createSettler(groundPosSettler,settlerSettings);

        map::vector3 groundPosDrink1(45,40,20);
        mAIDirector->createItem(groundPosDrink1,"Wine","Apple");

        map::vector3 groundPosDrink2(55,55,20);
        mAIDirector->createItem(groundPosDrink2,"Wine","Apple");

        map::vector3 groundPosDrink3(5,5,21);
        mAIDirector->createItem(groundPosDrink3,"Wine","Orange");

        map::vector3 groundPosFood1(45,45,20);
        mAIDirector->createItem(groundPosFood1,"Fruit","Apple");
    }

    void GameManager::addToSpawnList(std::shared_ptr<GameEntity> ent)
    {
        auto key=mNextGameObjectID++;
        ent->setID(key);
        mSpawnList.push_back(ent);
    }

    void GameManager::addToDeleteList(std::shared_ptr<GameEntity> ent)
    {
        auto iter=std::find_if(std::begin(mDeleteList),std::end(mDeleteList),[&ent](std::shared_ptr<GameEntity> const& value)
        {
            return (ent->ID()==value->ID());
        });

        if (iter==std::end(mDeleteList))
        {
            ent->pre_delete();

            mDeleteList.push_back(ent);

            //auto iterSpawn=std::find_if(std::begin(mSpawnList),std::end(mSpawnList),[&ent](std::shared_ptr<GameEntity> const& value)
            //{
            //    return (ent->ID()==value->ID());
            //});

            //if (iterSpawn!=std::end(mSpawnList))
            //    mSpawnList.erase(iterSpawn);
        }
    }

    const std::shared_ptr<GameManager> GameManager::instance()
    {
        static std::shared_ptr<GameManager> self;

        if (!self)
        {
            self = GameManager::create();
        }

        return self;
    }

    const std::shared_ptr<map::Region> GameManager::region() const
    {
        return mRegion;
    }

    const std::shared_ptr<properties::GameDefinition> GameManager::gameDefinition() const
    {
        return mGameDefs;
    }

    void GameManager::processSpawn()
    {
        for (auto ent : mSpawnList)
        {
            auto mapCell=this->region()->map()->cell(ent->position());
            ent->spawn(mapCell);
            mActiveList.push_back(ent);
        }

        mSpawnList.clear();
    }

    void GameManager::processUpdate(const float& dt)
    {
        for (auto ent : mActiveList)
        {
            if (ent->isValid())
                ent->update(dt);
        }
    }

    void GameManager::processPreUpdate()
    {
        for (auto ent : mActiveList)
        {
            if (ent->isValid())
                ent->pre_update();
        }
    }

    void GameManager::processPostUpdate()
    {
        auto map=GAME->region()->map();

        for (auto ent : mActiveList)
        {
            if (ent->toBeMoved())
            {
                map->cell(ent->position())->removeCharacter(std::dynamic_pointer_cast<game::Character>(ent));
                map->cell(ent->new_position())->addCharacter(std::dynamic_pointer_cast<game::Character>(ent));
                ent->update_position();
            }

            if (ent->isValid())
                ent->post_update();
        }
    }

    void GameManager::processDeletion()
    {
        for (auto ent : mDeleteList)
        {
            auto iter=std::find_if(std::begin(mActiveList),std::end(mActiveList),[&ent](std::shared_ptr<GameEntity> const& value)
            {
                return ent->ID()==value->ID();
            });

            if (iter!=std::end(mActiveList))
            {
                int index=std::distance(std::begin(mActiveList),iter);
                mActiveList.erase(iter);
            }

            auto mapCell=this->region()->map()->cell(ent->position());
            ent->destroy(mapCell);
        }

        mDeleteList.clear();
    }

    void GameManager::update(const float& dt)
    {
        this->processSpawn();
        mRegion->processSpawn();

        this->processPreUpdate();
        this->processUpdate(dt);
        this->processPostUpdate();

        this->processDeletion();
        mRegion->processDeletion();
    }
}