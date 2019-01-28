//
// Created by tatiana on 09.06.2018.
//

#include "StockManager.h"
#include "Item.h"
#include <algorithm>

namespace game
{
    StockManager::StockManager()
    {

    }

    StockManager::~StockManager()
    {

    }

    bool StockManager::isItemInStocks(std::shared_ptr<Item> item)
    {
        std::string id=item->itemID();
        auto iter_item_id=std::find_if(mItems.begin(),mItems.end(),[&id](properties::ItemsByQualityInfo const& elem)
        {
            return elem.itemID==id;
        });

        if (iter_item_id==mItems.end())
            return false;
        else
        {
            auto allItems=iter_item_id->itemsOfQualityOrHigher(game::ItemQuality::Poor);
            if (allItems.items.empty())
                return false;

            return allItems.isItemInStocks(item);
        }

        return false;
    }

    void StockManager::addUnstockedItem(std::shared_ptr<Item> item)
    {
        if (item->inStockpile())
            return;

        std::string id=item->itemID();
        auto iter_item_id=std::find_if(mUnstockedItems.begin(),mUnstockedItems.end(),
                [&id](properties::ItemsByQualityInfo const& elem)
        {
            return elem.itemID==id;
        });

        if (iter_item_id==mUnstockedItems.end())
        {
            properties::ItemsByQualityInfo newInfoByQuality;
            newInfoByQuality.itemID=id;

            properties::ItemsByQuality newItemsByQuality;
            newItemsByQuality.quality=item->quality();

            properties::ItemsByMaterialInfo newItemsByMaterialInfo;
            newItemsByMaterialInfo.materialID=item->materialID();
            newItemsByMaterialInfo.items.push_back(item);

            newItemsByQuality.items.items.push_back(newItemsByMaterialInfo);

            newInfoByQuality.items.push_back(newItemsByQuality);

            mUnstockedItems.push_back(newInfoByQuality);
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

                (*iter_item_id).items.push_back(newItemsByQuality);
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

                    (*iter_item_quality).items.items.push_back(newItemsByMaterialInfo);
                }
                else
                {
                    auto iter_item=std::find_if(iter_item_by_material->items.begin(),iter_item_by_material->items.end(),
                            [&item](std::shared_ptr<game::Item> const& elem)
                    {
                        return (elem->ID()==item->ID());
                    });

                    if (iter_item==iter_item_by_material->items.end())
                    {
                        (*iter_item_by_material).items.push_back(item);
                    }
                }
            }
        }
    }

    void StockManager::addItem(std::shared_ptr<Item> item)
    {
        std::string itemID=item->itemID();
        auto iter_item_id=std::find_if(mItems.begin(),mItems.end(),[&itemID](properties::ItemsByQualityInfo const& elem)
        {
            return elem.itemID==itemID;
        });

        if (iter_item_id==mItems.end())
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

            mItems.push_back(newInfoByQuality);
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

                (*iter_item_id).items.push_back(newItemsByQuality);
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

                    (*iter_item_quality).items.items.push_back(newItemsByMaterialInfo);
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
                        (*iter_item_by_material).items.push_back(item);
                    }
                }
            }
        }

        this->addUnstockedItem(item);
    }

    void StockManager::removeItem(std::shared_ptr<Item> item)
    {
        std::string itemID=item->itemID();
        auto iter_item_id=std::find_if(mItems.begin(),mItems.end(),[&itemID](properties::ItemsByQualityInfo const& elem)
        {
            return elem.itemID==itemID;
        });

        if (iter_item_id!=mItems.end())
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
                        (*iter_item_by_material).items.erase(iter_item);

                        if ((*iter_item_by_material).items.empty())
                        {
                            (*iter_item_quality).items.items.erase(iter_item_by_material);
                            if ((*iter_item_quality).items.items.empty())
                            {
                                (*iter_item_id).items.erase(iter_item_quality);
                                if ((*iter_item_id).items.empty())
                                {
                                    mItems.erase(iter_item_id);
                                }
                            }
                        }
                    }
                }
            }
        }

        this->removeUnstockedItem(item);
    }

    void StockManager::removeUnstockedItem(std::shared_ptr<Item> item)
    {
        std::string itemID=item->itemID();
        auto iter_item_id=std::find_if(mUnstockedItems.begin(),mUnstockedItems.end(),
                [&itemID](properties::ItemsByQualityInfo const& elem)
        {
            return elem.itemID==itemID;
        });

        if (iter_item_id!=mUnstockedItems.end())
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
                        (*iter_item_by_material).items.erase(iter_item);

                        if ((*iter_item_by_material).items.empty())
                        {
                            (*iter_item_quality).items.items.erase(iter_item_by_material);
                            if ((*iter_item_quality).items.items.empty())
                            {
                                (*iter_item_id).items.erase(iter_item_quality);
                                if ((*iter_item_id).items.empty())
                                {
                                    mUnstockedItems.erase(iter_item_id);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}