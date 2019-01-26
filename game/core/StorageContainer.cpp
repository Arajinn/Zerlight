//
// Created by tatiana on 22.06.2018.
//

#include "StorageContainer.h"
#include "Item.h"
#include "Stockpile.h"
#include "game/map/MapCell.h"

#include <algorithm>

namespace game
{
    StorageContainer::StorageContainer(const map::vector3& position)
            :Item(position)
    {

    }

    StorageContainer::~StorageContainer()
    {

    }

    std::shared_ptr<StorageContainer> StorageContainer::create(const map::vector3& position, const std::string& id, const MaterialID_t& materialID,
            std::shared_ptr<const properties::ItemDefinition> aItemDef)
    {
        std::shared_ptr<StorageContainer> ptr=std::make_shared<StorageContainer>(position);
        ptr->init(id, materialID,aItemDef);
        return ptr;
    }

    void StorageContainer::init(const std::string& id, const MaterialID_t& materialID, std::shared_ptr<const properties::ItemDefinition> aItemDef)
    {
        Item::init(id,materialID,aItemDef);
    }

    std::vector<std::shared_ptr<Item>> StorageContainer::containedResources() const
    {
        return mContainedResources;
    }

    int StorageContainer::containedResourcesCount() const
    {
        return mContainedResources.size();
    }

    void StorageContainer::removeItem(std::shared_ptr<Item> item)
    {
        auto iter=std::find_if(mContainedResources.begin(),mContainedResources.end(),[&item](std::shared_ptr<Item> const& elem)
        {
            return elem->ID()==item->ID();
        });

        if (iter==mContainedResources.end())
            return;

        mContainedResources.erase(iter);

        item->setParent(nullptr);

        const auto designation=cell()->designation();
        if (designation==nullptr)
            return;

        const auto stockpile=std::dynamic_pointer_cast<Stockpile>(designation);
        if (stockpile==nullptr)
            return;

        if (!item->inStockpile())
            return;

        item->setInStockpile(false);
        stockpile->stockStateChanged(item);
    }
}