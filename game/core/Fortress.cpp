//
// Created by tatiana on 09.06.2018.
//

#include "Fortress.h"
#include "defines.h"
#include "GameManager.h"
#include "Item.h"
#include "StockManager.h"
#include "Military.h"
#include "game/properties/GameDefinition.h"
#include "game/properties/ItemDefinition.h"

#include <algorithm>

namespace game
{
    Fortress::Fortress()
        :mStockWealth(0)
    {
        mStockManager=std::make_shared<StockManager>();
        mMilitary=std::make_shared<Military>();
    }

    Fortress::~Fortress()
    {

    }

    void Fortress::processSpawn()
    {
        for (const auto& elem : mSpawnList)
        {
            addItemToContainer(elem);
        }

        mSpawnList.clear();
    }

    void Fortress::processDeletion()
    {
        for (const auto& elem : mDeleteList)
        {
            removeItemFromContainer(elem);
        }

        mDeleteList.clear();
    }

    void Fortress::addEntity(ItemEffectType type, std::shared_ptr<GameEntity> ent)
    {
        auto iter_effect=std::find_if(mEntsWithEffects.begin(),mEntsWithEffects.end(),
                [&type](entity_effect_info const& elem)
        {
            return elem.type==type;
        });

        if (iter_effect==mEntsWithEffects.end())
        {
            entity_effect_info info;
            info.type=type;
            info.entities.push_back(ent);
            mEntsWithEffects.push_back(info);
        }
        else
        {
            auto iter_item=std::find_if(iter_effect->entities.begin(),iter_effect->entities.end(),
                    [&ent](std::shared_ptr<GameEntity> const& elem)
            {
                return elem->ID()==ent->ID();
            });

            if (iter_item==iter_effect->entities.end())
                iter_effect->entities.push_back(ent);
        }
    }

    void Fortress::trackEntity(std::shared_ptr<Item> item)
    {
        auto itemDef = GAME_DEFINITIONS->itemDefinition(item->itemID());
        auto effects=itemDef->Effects;
        for (const auto& effect : effects)
        {
            addEntity(effect.Effect,item);
        }
    }

    void Fortress::trackItem(std::shared_ptr<Item> item)
    {
        trackEntity(item);

        if (mStockManager->isItemInStocks(item))
            return;

        mStockWealth+=item->value();
    }

    void Fortress::addItem(std::shared_ptr<Item> item)
    {
        auto iter=std::find_if(mSpawnList.begin(),mSpawnList.end(),[&item](std::shared_ptr<Item> const& elem)
        {
            return item->ID()==elem->ID();
        });

        if (iter==mSpawnList.end())
            mSpawnList.push_back(item);
    }

    void Fortress::addItemToContainer(std::shared_ptr<Item> item)
    {
        trackItem(item);
        mStockManager->addItem(item);
    }

    std::vector<std::shared_ptr<GameEntity>> Fortress::getEntities(ItemEffectType type)
    {
        auto iter_effect=std::find_if(mEntsWithEffects.begin(),mEntsWithEffects.end(),[&type](entity_effect_info const& elem)
        {
            return elem.type==type;
        });

        std::vector<std::shared_ptr<GameEntity>> result;
        if (iter_effect==mEntsWithEffects.end())
            return result;
        else
            return iter_effect->entities;
    }

    void Fortress::stopTrackingEntity(std::shared_ptr<GameEntity> ent, std::vector<properties::ItemEffect> effects)
    {
        for (auto effect : effects)
        {
            auto iter_effect=std::find_if(mEntsWithEffects.begin(),mEntsWithEffects.end(),
                    [&effect](entity_effect_info const& elem)
            {
                return elem.type==effect.Effect;
            });

            if (iter_effect!=mEntsWithEffects.end())
            {
                auto iter_item=std::find_if(iter_effect->entities.begin(),iter_effect->entities.end(),
                        [&ent](std::shared_ptr<GameEntity> const& elem)
                {
                    return elem->ID()==ent->ID();
                });

                if (iter_item!=iter_effect->entities.end())
                {
                    (*iter_effect).entities.erase(iter_item);
                    if ((*iter_effect).entities.empty())
                    {
                        mEntsWithEffects.erase(iter_effect);
                    }
                }
            }
        }
    }

    void Fortress::stopTrackingItem(std::shared_ptr<Item> item)
    {
        //auto itemDef=GAME_DEFINITIONS->itemDefinition(item->itemID());
        const auto itemDef=item->itemDef();
        stopTrackingEntity(item,itemDef->Effects);

        if (!mStockManager->isItemInStocks(item))
            return;

        mStockWealth-=std::min(item->value(),mStockWealth);
    }

    void Fortress::removeItemFromContainer(std::shared_ptr<game::Item> item)
    {
        stopTrackingItem(item);
        mStockManager->removeItem(item);
    }

    void Fortress::removeItem(std::shared_ptr<Item> item)
    {
        auto iter=std::find_if(mDeleteList.begin(),mDeleteList.end(),[&item](std::shared_ptr<Item> const& elem)
        {
            return item->ID()==elem->ID();
        });

        if (iter==mDeleteList.end())
            mDeleteList.push_back(item);
    }

    std::shared_ptr<Military> Fortress::military() const
    {
        return mMilitary;
    }
}