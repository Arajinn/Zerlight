//
// Created by tatiana on 26.03.2018.
//

#include "GameManager.h"
#include "defines.h"
#include "Character.h"
#include "Construction.h"
#include "Item.h"
#include "AIDirector.h"
#include "Stairs.h"
#include "CreateWorldOptions.h"
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
        auto ptr=std::make_shared<GameManager>();
        ptr->init();
        return ptr;
    }

    void GameManager::init()
    {
        mNextGameObjectID=0;
        mNextNodeID=0;
        mNextNavGraphID=0;

        mRegion=std::make_shared<map::Region>();
        mAIDirector=std::make_shared<AIDirector>(shared_from_this());
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

    bool GameManager::testSpawn()
    {
        const auto worldOptions=std::make_shared<CreateWorldOptions>();
        worldOptions->KingdomName="Test kingdom";
        mAIDirector->generateStartingFactions(worldOptions);

//        map::vector3 groundPosStair(20,20,20);
//        auto entStair=Stairs::create(groundPosStair,"SoilStairs");
//        this->addToSpawnList(entStair);

        const map::vector3 groundPosSettler(56,43,20);
        properties::SettlerSettings settlerSettings;
        settlerSettings.RaceID="Gnome";
        settlerSettings.offset=map::vector3(0,0,0);
        settlerSettings.Profession="";
        mAIDirector->createSettler(groundPosSettler,settlerSettings);

        const map::vector3 groundPosBear(15,43,20);
        const map::vector3 groundOffsetBear(0,0,0);
        mAIDirector->createWildAnimal(groundPosBear,groundOffsetBear,"Bear");

        map::vector3 groundPosDrink1(45,40,20);
        mAIDirector->createItem(groundPosDrink1,"Wine","Apple");

        map::vector3 groundPosDrink2(55,55,20);
        mAIDirector->createItem(groundPosDrink2,"Wine","Apple");

        map::vector3 groundPosDrink3(5,5,21);
        mAIDirector->createItem(groundPosDrink3,"Wine","Grape");

        map::vector3 groundPosFood1(45,45,20);
        mAIDirector->createItem(groundPosFood1,"Fruit","Apple");

        map::vector3 groundPosFood2(15,15,21);
        mAIDirector->createItem(groundPosFood2,"Fruit","Grape");

        return true;
    }

    void GameManager::addToSpawnList(std::shared_ptr<GameEntity> ent)
    {
        auto key=mNextGameObjectID++;
        ent->setID(key);
        mSpawnList.push_back(ent);
    }

    void GameManager::addToDeleteList(std::shared_ptr<GameEntity> ent)
    {
        auto iter=std::find_if(mDeleteList.begin(),mDeleteList.end(),[&ent](std::shared_ptr<GameEntity> const& elem)
        {
            return (ent->ID()==elem->ID());
        });

        if (iter==mDeleteList.end())
        {
            //ent->pre_delete();

            mDeleteList.push_back(ent);

            //auto iterSpawn=std::find_if(std::begin(mSpawnList),std::end(mSpawnList),[&ent](std::shared_ptr<GameEntity> const& value)
            //{
            //    return (ent->ID()==value->ID());
            //});

            //if (iterSpawn!=std::end(mSpawnList))
            //    mSpawnList.erase(iterSpawn);
        }
    }

    std::shared_ptr<map::Region> GameManager::region() const
    {
        return mRegion;
    }

    std::shared_ptr<AIDirector> GameManager::aiDirector() const
    {
        return mAIDirector;
    }

    void GameManager::processSpawn()
    {
        for (auto ent : mSpawnList)
        {
            auto mapCell=this->region()->map()->cell(ent->position());
            ent->spawn(mapCell);

            if (ent->isUpdatable())
                mActiveList.push_back(ent);
            else
                mNonActiveList.push_back(ent);
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
        auto map=WORLD_MAP;

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
            if (ent->isUpdatable())
            {
                auto iter = std::find_if(mActiveList.begin(), mActiveList.end(),
                                         [&ent](std::shared_ptr<GameEntity> const &elem)
                                         {
                                             return ent->ID() == elem->ID();
                                         });

                if (iter != mActiveList.end())
                {
                    mActiveList.erase(iter);
                }
            }
            else
            {
                auto iter = std::find_if(mNonActiveList.begin(), mNonActiveList.end(),
                                         [&ent](std::shared_ptr<GameEntity> const &elem)
                                         {
                                             return ent->ID() == elem->ID();
                                         });

                if (iter != mNonActiveList.end())
                {
                    mNonActiveList.erase(iter);
                }
            }

            auto mapCell=this->region()->map()->cell(ent->position());
            ent->pre_delete();
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

    bool GameManager::generateNewWorld()
    {
        mIsGenerateFinish=false;

        mRegion->map()->testMap();

        mRegion->map()->postInit();

        this->testSpawn();

        this->update(0.0f);

        mIsGenerateFinish=true;
    }

    float GameManager::getInitProgress()
    {
        return mRegion->map()->getInitProgress();
    }

    bool GameManager::getIsGenerateFinish() const
    {
        return mIsGenerateFinish;
    }
}