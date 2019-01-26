//
// Created by tatiana on 27.12.18.
//

#include "Stockpile.h"
#include "Item.h"
#include "StorageContainer.h"
#include "StockItemJob.h"

#include <algorithm>

namespace game
{
    Stockpile::Stockpile()
        :Designation()
    {
        mThreshold=0;
        mTotalCells=0;
        mSuspended=false;
        mUnsuspendAtThreshold=false;
    }

    Stockpile::~Stockpile()
    {

    }

    int Stockpile::stockedCount() const
    {
        int result=0;

        for (const auto elem1 : mStockedItems)
        {
            for (const auto elem2 : elem1.items)
            {
                for (const auto elem3 : elem2.items.items)
                {
                    result+=elem3.items.size();
                }
            }
        }

        return result;
    }

    int Stockpile::potentialCount() const
    {
        int result=0;

        for (const auto container : mStorageContainers)
        {
            result+=container->effectAmount(ItemEffectType::Storage)-1;
        }

        return result+mTotalCells;
    }

    void Stockpile::setSuspend(bool value)
    {
        if ((value) && (!mSuspended))
        {
            for (const auto job : mStockJobs)
                job->removeJob();
        }

        mSuspended=value;

        if (mSuspended)
            return;

        mUnsuspendAtThreshold=false;
    }

    void Stockpile::addStockedItem(std::shared_ptr<Item> item)
    {
        if (!item->inStockpile())
            return;

        std::string itemID=item->itemID();
        auto iter_item_id=std::find_if(mStockedItems.begin(),mStockedItems.end(),
                [&itemID](properties::ItemsByQualityInfo const& elem)
        {
            return elem.itemID==itemID;
        });

        if (iter_item_id==mStockedItems.end())
        {
            properties::ItemsByQualityInfo newInfoByQuality;
            newInfoByQuality.itemID=itemID;

            properties::ItemsByQuality newItemsByQuality;
            newItemsByQuality.quality=item->quality();

            properties::ItemsByMaterialInfo newItemsByMaterialInfo;
            newItemsByMaterialInfo.materialID=item->materialID();
            newItemsByMaterialInfo.items.push_back(item);

            newItemsByQuality.items.items.push_back(newItemsByMaterialInfo);

            newInfoByQuality.items.push_back(newItemsByQuality);

            mStockedItems.push_back(newInfoByQuality);
        }
        else
        {
            ItemQuality quality=item->quality();
            auto iter_item_quality=std::find_if(iter_item_id->items.begin(),iter_item_id->items.end(),
                    [&quality](properties::ItemsByQuality const& elem)
            {
                return (elem.quality==quality);
            });

            if (iter_item_quality==iter_item_id->items.end())
            {
                properties::ItemsByQuality newItemsByQuality;
                newItemsByQuality.quality=item->quality();

                properties::ItemsByMaterialInfo newItemsByMaterialInfo;
                newItemsByMaterialInfo.materialID=item->materialID();
                newItemsByMaterialInfo.items.push_back(item);

                newItemsByQuality.items.items.push_back(newItemsByMaterialInfo);

                const auto index=std::distance(mStockedItems.begin(),iter_item_id);
                mStockedItems[index].items.push_back(newItemsByQuality);
            }
            else
            {
                const auto materialID=item->materialID();
                auto iter_item_by_material=std::find_if(iter_item_quality->items.items.begin(),iter_item_quality->items.items.end(),
                        [&materialID](properties::ItemsByMaterialInfo const& elem)
                {
                    return (elem.materialID==materialID);
                });

                if (iter_item_by_material==iter_item_quality->items.items.end())
                {
                    properties::ItemsByMaterialInfo newItemsByMaterialInfo;
                    newItemsByMaterialInfo.materialID=item->materialID();
                    newItemsByMaterialInfo.items.push_back(item);

                    const auto index1=std::distance(mStockedItems.begin(),iter_item_id);
                    const auto index2=std::distance(iter_item_id->items.begin(),iter_item_quality);
                    mStockedItems[index1].items[index2].items.items.push_back(newItemsByMaterialInfo);
                }
                else
                {
                    int id=item->ID();
                    auto iter_item=std::find_if(iter_item_by_material->items.begin(),iter_item_by_material->items.end(),
                            [&id](std::shared_ptr<game::Item> const& elem)
                    {
                        return (elem->ID()==id);
                    });

                    if (iter_item==iter_item_by_material->items.end())
                    {
                        const auto index1=std::distance(mStockedItems.begin(),iter_item_id);
                        const auto index2=std::distance(iter_item_id->items.begin(),iter_item_quality);
                        const auto index3=std::distance(iter_item_quality->items.items.begin(),iter_item_by_material);
                        mStockedItems[index1].items[index2].items.items[index3].items.push_back(item);
                    }
                }
            }
        }

        if (mThreshold==0)
            return;

        if (stockedCount()!=potentialCount())
            return;

        setSuspend(true);
        mUnsuspendAtThreshold=true;
    }

    void Stockpile::removeStockedItem(std::shared_ptr<Item> item)
    {
        if (item->inStockpile())
            return;

        std::string itemID=item->itemID();
        auto iter_item_id=std::find_if(mStockedItems.begin(),mStockedItems.end(),
                [&itemID](properties::ItemsByQualityInfo const& elem)
        {
            return elem.itemID==itemID;
        });

        if (iter_item_id!=mStockedItems.end())
        {
            ItemQuality quality=item->quality();
            auto iter_item_quality=std::find_if(iter_item_id->items.begin(),iter_item_id->items.end(),
                    [&quality](properties::ItemsByQuality const& elem)
            {
                return (elem.quality==quality);
            });

            if (iter_item_quality!=iter_item_id->items.end())
            {
                const auto materialID=item->materialID();
                auto iter_item_by_material=std::find_if(iter_item_quality->items.items.begin(),iter_item_quality->items.items.end(),
                        [&materialID](properties::ItemsByMaterialInfo const& elem)
                {
                    return (elem.materialID==materialID);
                });

                if (iter_item_by_material!=iter_item_quality->items.items.end())
                {
                    int id=item->ID();
                    auto iter_item=std::find_if(iter_item_by_material->items.begin(),iter_item_by_material->items.end(),
                            [&id](std::shared_ptr<game::Item> const& elem)
                    {
                        return (elem->ID()==id);
                    });

                    if (iter_item!=iter_item_by_material->items.end())
                    {
                        const auto index1=std::distance(mStockedItems.begin(),iter_item_id);
                        const auto index2=std::distance(iter_item_id->items.begin(),iter_item_quality);
                        const auto index3=std::distance(iter_item_quality->items.items.begin(),iter_item_by_material);

                        mStockedItems[index1].items[index2].items.items[index3].items.erase(iter_item);

                        if (mStockedItems[index1].items[index2].items.items[index3].items.empty())
                        {
                            mStockedItems[index1].items[index2].items.items.erase(iter_item_by_material);
                            if (mStockedItems[index1].items[index2].items.items.empty())
                            {
                                mStockedItems[index1].items.erase(iter_item_quality);
                                if (mStockedItems[index1].items.empty())
                                {
                                    mStockedItems.erase(iter_item_id);
                                }
                            }
                        }
                    }
                }
            }
        }

        const auto cast_to_storage=std::dynamic_pointer_cast<StorageContainer>(item);
        if (cast_to_storage!=nullptr)
        {
            const auto contained=cast_to_storage->containedResources();
            for (const auto contain : contained)
                contain->setInStockpile(false);
        }

        if (mThreshold==0)
            return;

        if (!mUnsuspendAtThreshold)
            return;

        if ((potentialCount()-stockedCount())<mThreshold)
            return;

        setSuspend(false);
    }

    void Stockpile::stockStateChanged(std::shared_ptr<Item> item)
    {
        if (item->inStockpile())
            addStockedItem(item);
        else
            removeStockedItem(item);
    }
}