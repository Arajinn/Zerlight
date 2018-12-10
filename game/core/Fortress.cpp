//
// Created by tatiana on 09.06.2018.
//

#include "Fortress.h"
#include "defines.h"
#include "GameManager.h"
#include "Item.h"
#include "StockManager.h"
#include "game/properties/GameDefinition.h"
#include "game/properties/ItemDefinition.h"

#include <algorithm>

namespace game
{
    Fortress::Fortress()
        :mStockWealth(0)
    {
        mStockManager=std::shared_ptr<StockManager>(new StockManager());
    }

    Fortress::~Fortress()
    {

    }

    void Fortress::processSpawn()
    {
        for (auto elem : mSpawnList)
        {
            this->addItemToContainer(elem);
        }

        mSpawnList.clear();
    }

    void Fortress::processDeletion()
    {
        for (auto elem : mDeleteList)
        {
            this->removeItemFromContainer(elem);
        }

        mDeleteList.clear();
    }

    void Fortress::addEntity(ItemEffectType type, std::shared_ptr<GameEntity> ent)
    {
        auto iter_effect=std::find_if(std::begin(mEntsWithEffects),std::end(mEntsWithEffects),[&type](entity_effect_info const& value)
        {
            return value.type==type;
        });

        if (iter_effect==std::end(mEntsWithEffects))
        {
            entity_effect_info info;
            info.type=type;
            info.entities.push_back(ent);
            mEntsWithEffects.push_back(info);
        }
        else
        {
            auto iter_item=std::find_if(std::begin(iter_effect->entities),std::end(iter_effect->entities),[&ent](std::shared_ptr<GameEntity> const& value)
            {
                return value->ID()==ent->ID();
            });

            if (iter_item==std::end(iter_effect->entities))
                iter_effect->entities.push_back(ent);
        }
    }

    void Fortress::trackEntity(std::shared_ptr<Item> item)
    {
        auto itemDef = GAME_DEFINITIONS->itemDefinition(item->itemID());
        auto effects=itemDef->Effects;
        for (auto effect : effects)
        {
            this->addEntity(effect.Effect,item);
        }
    }

    void Fortress::trackItem(std::shared_ptr<Item> item)
    {
        this->trackEntity(item);

        if (mStockManager->isItemInStocks(item))
            return;

        mStockWealth+=item->value();
    }

    void Fortress::addItem(std::shared_ptr<Item> item)
    {
        auto iter=std::find_if(std::begin(mSpawnList),std::end(mSpawnList),[&item](std::shared_ptr<Item> const& value)
        {
            return item->ID()==value->ID();
        });

        if (iter==std::end(mSpawnList))
            mSpawnList.push_back(item);
    }

    void Fortress::addItemToContainer(std::shared_ptr<Item> item)
    {
        this->trackItem(item);
        mStockManager->addItem(item);
    }

    std::vector<std::shared_ptr<GameEntity>> Fortress::getEntities(ItemEffectType type)
    {
        auto iter_effect=std::find_if(std::begin(mEntsWithEffects),std::end(mEntsWithEffects),[&type](entity_effect_info const& value)
        {
            return value.type==type;
        });

        std::vector<std::shared_ptr<GameEntity>> result;
        if (iter_effect==std::end(mEntsWithEffects))
            return result;
        else
            return iter_effect->entities;
    }

    void Fortress::stopTrackingEntity(std::shared_ptr<GameEntity> ent, std::vector<properties::ItemEffect> effects)
    {
        for (auto effect : effects)
        {
            auto iter_effect=std::find_if(std::begin(mEntsWithEffects),std::end(mEntsWithEffects),[&effect](entity_effect_info const& value)
            {
                return value.type==effect.Effect;
            });

            if (iter_effect!=std::end(mEntsWithEffects))
            {
                auto iter_item=std::find_if(std::begin(iter_effect->entities),std::end(iter_effect->entities),[&ent](std::shared_ptr<GameEntity> const& value)
                {
                    return value->ID()==ent->ID();
                });

                if (iter_item!=std::end(iter_effect->entities))
                {
                    int index=std::distance(std::begin(mEntsWithEffects),iter_effect);
                    mEntsWithEffects[index].entities.erase(iter_item);
                    if (mEntsWithEffects[index].entities.size()==0)
                    {
                        mEntsWithEffects.erase(iter_effect);
                    }
                }
            }
        }
    }

    void Fortress::stopTrackingItem(std::shared_ptr<Item> item)
    {
        auto itemDef=GAME_DEFINITIONS->itemDefinition(item->itemID());
        this->stopTrackingEntity(item,itemDef->Effects);

        if (!mStockManager->isItemInStocks(item))
            return;

        mStockWealth-=std::min(item->value(),mStockWealth);
    }

    void Fortress::removeItemFromContainer(std::shared_ptr<game::Item> item)
    {
        this->stopTrackingItem(item);
        mStockManager->removeItem(item);
    }

    void Fortress::removeItem(std::shared_ptr<Item> item)
    {
        auto iter=std::find_if(std::begin(mDeleteList),std::end(mDeleteList),[&item](std::shared_ptr<Item> const& value)
        {
            return item->ID()==value->ID();
        });

        if (iter==std::end(mDeleteList))
            mDeleteList.push_back(item);
    }
}