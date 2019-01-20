//
// Created by tatiana on 27.12.18.
//

#ifndef ZERLIGHT_STOCKPILE_H
#define ZERLIGHT_STOCKPILE_H

#include "game/core/Designation.h"
#include "game/properties/ItemSettings.h"

#include <vector>
#include <memory>

namespace game
{
    class Item;
    class StorageContainer;
    class StockItemJob;
}

namespace game {
    class Stockpile : public Designation{
    public:
        Stockpile();
        ~Stockpile();

        void setSuspend(bool value);

        void stockStateChanged(std::shared_ptr<Item> item);
        void addStockedItem(std::shared_ptr<Item> item);
        void removeStockedItem(std::shared_ptr<Item> item);
    private:
        std::vector<properties::ItemsByQualityInfo> mStockedItems;
        unsigned int mThreshold;
        std::vector<std::shared_ptr<StorageContainer>> mStorageContainers;
        int mTotalCells;
        bool mSuspended;
        bool mUnsuspendAtThreshold;
        std::vector<std::shared_ptr<StockItemJob>> mStockJobs;

        int stockedCount() const;
        int potentialCount() const;


    };
}

#endif //ZERLIGHT_STOCKPILE_H
