//
// Created by tatiana.polozova on 09.06.2018.
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
        auto iter_item_id=std::find_if(std::begin(mItems),std::end(mItems),[&id](properties::ItemsByQualityInfo const& value)
        {
            return value.itemID==id;
        });

        if (iter_item_id==std::end(mItems))
            return false;
        else
        {
            auto allItems=iter_item_id->itemsOfQualityOrHigher(game::ItemQuality::Poor);
            if (allItems.items.size()==0)
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
        auto iter_item_id=std::find_if(std::begin(mUnstockedItems),std::end(mUnstockedItems),[&id](properties::ItemsByQualityInfo const& value)
        {
            return value.itemID==id;
        });

        if (iter_item_id==std::end(mUnstockedItems))
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
            auto iter_item_quality=std::find_if(std::begin(iter_item_id->items),std::end(iter_item_id->items),[&quality](properties::ItemsByQuality const& value)
            {
                return (value.quality==quality);
            });

            if (iter_item_quality==std::end(iter_item_id->items))
            {
                properties::ItemsByQuality newItemsByQuality;
                newItemsByQuality.quality=item->quality();

                properties::ItemsByMaterialInfo newItemsByMaterialInfo;
                newItemsByMaterialInfo.materialID=item->materialID();
                newItemsByMaterialInfo.items.push_back(item);

                newItemsByQuality.items.items.push_back(newItemsByMaterialInfo);

                int index=std::distance(std::begin(mUnstockedItems),iter_item_id);
                mUnstockedItems[index].items.push_back(newItemsByQuality);
            }
            else
            {
                std::string materialID=item->materialID();
                auto iter_item_by_material=std::find_if(std::begin(iter_item_quality->items.items),std::end(iter_item_quality->items.items),[&materialID](properties::ItemsByMaterialInfo const& value)
                {
                    return (value.materialID==materialID);
                });

                if (iter_item_by_material==std::end(iter_item_quality->items.items))
                {
                    properties::ItemsByMaterialInfo newItemsByMaterialInfo;
                    newItemsByMaterialInfo.materialID=item->materialID();
                    newItemsByMaterialInfo.items.push_back(item);

                    int index1=std::distance(std::begin(mUnstockedItems),iter_item_id);
                    int index2=std::distance(std::begin(iter_item_id->items),iter_item_quality);
                    mUnstockedItems[index1].items[index2].items.items.push_back(newItemsByMaterialInfo);
                }
                else
                {
                    auto iter_item=std::find_if(std::begin(iter_item_by_material->items),std::end(iter_item_by_material->items),[&item](std::shared_ptr<game::Item> const& value)
                    {
                        return (value->ID()==item->ID());
                    });

                    if (iter_item==std::end(iter_item_by_material->items))
                    {
                        int index1=std::distance(std::begin(mUnstockedItems),iter_item_id);
                        int index2=std::distance(std::begin(iter_item_id->items),iter_item_quality);
                        int index3=std::distance(std::begin(iter_item_quality->items.items),iter_item_by_material);
                        mUnstockedItems[index1].items[index2].items.items[index3].items.push_back(item);
                    }
                }
            }
        }
    }

    void StockManager::addItem(std::shared_ptr<Item> item)
    {
        std::string itemID=item->itemID();
        auto iter_item_id=std::find_if(std::begin(mItems),std::end(mItems),[&itemID](properties::ItemsByQualityInfo const& value)
        {
            return value.itemID==itemID;
        });

        if (iter_item_id==std::end(mItems))
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
            auto iter_item_quality=std::find_if(std::begin(iter_item_id->items),std::end(iter_item_id->items),[&quality](properties::ItemsByQuality const& value)
            {
                return (value.quality==quality);
            });

            if (iter_item_quality==std::end(iter_item_id->items))
            {
                properties::ItemsByQuality newItemsByQuality;
                newItemsByQuality.quality=item->quality();

                properties::ItemsByMaterialInfo newItemsByMaterialInfo;
                newItemsByMaterialInfo.materialID=item->materialID();
                newItemsByMaterialInfo.items.push_back(item);

                newItemsByQuality.items.items.push_back(newItemsByMaterialInfo);

                int index=std::distance(std::begin(mItems),iter_item_id);
                mItems[index].items.push_back(newItemsByQuality);
            }
            else
            {
                std::string materialID=item->materialID();
                auto iter_item_by_material=std::find_if(std::begin(iter_item_quality->items.items),std::end(iter_item_quality->items.items),[&materialID](properties::ItemsByMaterialInfo const& value)
                {
                    return (value.materialID==materialID);
                });

                if (iter_item_by_material==std::end(iter_item_quality->items.items))
                {
                    properties::ItemsByMaterialInfo newItemsByMaterialInfo;
                    newItemsByMaterialInfo.materialID=item->materialID();
                    newItemsByMaterialInfo.items.push_back(item);

                    int index1=std::distance(std::begin(mItems),iter_item_id);
                    int index2=std::distance(std::begin(iter_item_id->items),iter_item_quality);
                    mItems[index1].items[index2].items.items.push_back(newItemsByMaterialInfo);
                }
                else
                {
                    int id=item->ID();
                    auto iter_item=std::find_if(std::begin(iter_item_by_material->items),std::end(iter_item_by_material->items),[&id](std::shared_ptr<game::Item> const& value)
                    {
                        return (value->ID()==id);
                    });

                    if (iter_item==std::end(iter_item_by_material->items))
                    {
                        int index1=std::distance(std::begin(mItems),iter_item_id);
                        int index2=std::distance(std::begin(iter_item_id->items),iter_item_quality);
                        int index3=std::distance(std::begin(iter_item_quality->items.items),iter_item_by_material);
                        mItems[index1].items[index2].items.items[index3].items.push_back(item);
                    }
                }
            }
        }

        this->addUnstockedItem(item);
    }

    void StockManager::removeItem(std::shared_ptr<Item> item)
    {
        std::string itemID=item->itemID();
        auto iter_item_id=std::find_if(std::begin(mItems),std::end(mItems),[&itemID](properties::ItemsByQualityInfo const& value)
        {
            return value.itemID==itemID;
        });

        if (iter_item_id!=std::end(mItems))
        {
            ItemQuality quality=item->quality();
            auto iter_item_quality=std::find_if(std::begin(iter_item_id->items),std::end(iter_item_id->items),[&quality](properties::ItemsByQuality const& value)
            {
                return (value.quality==quality);
            });

            if (iter_item_quality!=std::end(iter_item_id->items))
            {
                std::string materialID=item->materialID();
                auto iter_item_by_material=std::find_if(std::begin(iter_item_quality->items.items),std::end(iter_item_quality->items.items),[&materialID](properties::ItemsByMaterialInfo const& value)
                {
                    return (value.materialID==materialID);
                });

                if (iter_item_by_material!=std::end(iter_item_quality->items.items))
                {
                    int id=item->ID();
                    auto iter_item=std::find_if(std::begin(iter_item_by_material->items),std::end(iter_item_by_material->items),[&id](std::shared_ptr<game::Item> const& value)
                    {
                        return (value->ID()==id);
                    });

                    if (iter_item!=std::end(iter_item_by_material->items))
                    {
                        int index1=std::distance(std::begin(mItems),iter_item_id);
                        int index2=std::distance(std::begin(iter_item_id->items),iter_item_quality);
                        int index3=std::distance(std::begin(iter_item_quality->items.items),iter_item_by_material);

                        mItems[index1].items[index2].items.items[index3].items.erase(iter_item);

                        if (mItems[index1].items[index2].items.items[index3].items.size()==0)
                        {
                            mItems[index1].items[index2].items.items.erase(iter_item_by_material);
                            if (mItems[index1].items[index2].items.items.size()==0)
                            {
                                mItems[index1].items.erase(iter_item_quality);
                                if (mItems[index1].items.size()==0)
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
        auto iter_item_id=std::find_if(std::begin(mUnstockedItems),std::end(mUnstockedItems),[&itemID](properties::ItemsByQualityInfo const& value)
        {
            return value.itemID==itemID;
        });

        if (iter_item_id!=std::end(mUnstockedItems))
        {
            ItemQuality quality=item->quality();
            auto iter_item_quality=std::find_if(std::begin(iter_item_id->items),std::end(iter_item_id->items),[&quality](properties::ItemsByQuality const& value)
            {
                return (value.quality==quality);
            });

            if (iter_item_quality!=std::end(iter_item_id->items))
            {
                std::string materialID=item->materialID();
                auto iter_item_by_material=std::find_if(std::begin(iter_item_quality->items.items),std::end(iter_item_quality->items.items),[&materialID](properties::ItemsByMaterialInfo const& value)
                {
                    return (value.materialID==materialID);
                });

                if (iter_item_by_material!=std::end(iter_item_quality->items.items))
                {
                    int id=item->ID();
                    auto iter_item=std::find_if(std::begin(iter_item_by_material->items),std::end(iter_item_by_material->items),[&id](std::shared_ptr<game::Item> const& value)
                    {
                        return (value->ID()==id);
                    });

                    if (iter_item!=std::end(iter_item_by_material->items))
                    {
                        int index1=std::distance(std::begin(mUnstockedItems),iter_item_id);
                        int index2=std::distance(std::begin(iter_item_id->items),iter_item_quality);
                        int index3=std::distance(std::begin(iter_item_quality->items.items),iter_item_by_material);

                        mUnstockedItems[index1].items[index2].items.items[index3].items.erase(iter_item);

                        if (mUnstockedItems[index1].items[index2].items.items[index3].items.size()==0)
                        {
                            mUnstockedItems[index1].items[index2].items.items.erase(iter_item_by_material);
                            if (mUnstockedItems[index1].items[index2].items.items.size()==0)
                            {
                                mUnstockedItems[index1].items.erase(iter_item_quality);
                                if (mUnstockedItems[index1].items.size()==0)
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