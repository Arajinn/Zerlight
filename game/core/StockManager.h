//
// Created by tatiana.polozova on 09.06.2018.
//

#ifndef ZERL_STOCKMANAGER_H
#define ZERL_STOCKMANAGER_H

#include "game/properties/ItemSettings.h"

#include <memory>

namespace game
{
    class Item;
}

namespace game {
    class StockManager {
    public:
        StockManager();
        ~StockManager();

        bool isItemInStocks(std::shared_ptr<Item> item);
        void addItem(std::shared_ptr<Item> item);
        void removeItem(std::shared_ptr<Item> item);
    private:
        std::vector<properties::ItemsByQualityInfo> mItems;
        std::vector<properties::ItemsByQualityInfo> mUnstockedItems;

        void addUnstockedItem(std::shared_ptr<Item> item);
        void removeUnstockedItem(std::shared_ptr<Item> item);
    };
}

#endif //ZERL_STOCKMANAGER_H
