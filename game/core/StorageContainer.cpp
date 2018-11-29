//
// Created by tatiana.polozova on 22.06.2018.
//

#include "StorageContainer.h"

namespace game
{
    StorageContainer::StorageContainer(const map::vector3& position)
            :Item(position)
    {

    }

    StorageContainer::~StorageContainer()
    {

    }

    std::shared_ptr<StorageContainer> StorageContainer::create(const map::vector3& position, std::string id, std::string materialID, std::shared_ptr<properties::ItemDefinition> aItemDef)
    {
        std::shared_ptr<StorageContainer> ptr=std::shared_ptr<StorageContainer>(new StorageContainer(position));
        ptr->init(id, materialID,aItemDef);
        return ptr;
    }

    void StorageContainer::init(std::string id, std::string materialID, std::shared_ptr<properties::ItemDefinition> aItemDef)
    {
        Item::init(id,materialID,aItemDef);
    }
}