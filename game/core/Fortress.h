//
// Created by tatiana on 09.06.2018.
//

#ifndef ZERL_FORTRESS_H
#define ZERL_FORTRESS_H

#include "game/properties/ItemDefinition.h"

#include <memory>
#include <vector>

namespace game
{
    class Item;
    class GameEntity;
    class StockManager;
}

namespace game {
    class Fortress {
    public:
        struct entity_effect_info
        {
            ItemEffectType type;
            std::vector<std::shared_ptr<GameEntity>> entities;
        };

        Fortress();
        ~Fortress();

        void processSpawn();
        void processDeletion();

        void addItem(std::shared_ptr<Item> item);
        void removeItem(std::shared_ptr<Item> item);

        std::vector<std::shared_ptr<GameEntity>> getEntities(ItemEffectType type);
    private:
        std::shared_ptr<StockManager> mStockManager;
        unsigned int mStockWealth;

        void trackEntity(std::shared_ptr<Item> item);
        void trackItem(std::shared_ptr<Item> item);

        std::vector<entity_effect_info> mEntsWithEffects;
        std::vector<std::shared_ptr<Item>> mSpawnList;
        std::vector<std::shared_ptr<Item>> mDeleteList;

        void addEntity(ItemEffectType type, std::shared_ptr<GameEntity> ent);

        void stopTrackingEntity(std::shared_ptr<GameEntity> ent, std::vector<properties::ItemEffect> effects);
        void stopTrackingItem(std::shared_ptr<Item> item);

        void addItemToContainer(std::shared_ptr<Item> item);
        void removeItemFromContainer(std::shared_ptr<Item> item);
    };
}

#endif //ZERL_FORTRESS_H
