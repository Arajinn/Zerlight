//
// Created by tatiana.polozova on 22.06.2018.
//

#include "ResourcePile.h"

namespace game
{
    ResourcePile::ResourcePile(const map::vector3& position)
            :StorageContainer(position)
    {

    }

    ResourcePile::~ResourcePile()
    {

    }

    std::shared_ptr<ResourcePile> ResourcePile::create(const map::vector3& position, std::string id, std::string materialID, std::shared_ptr<properties::ItemDefinition> aItemDef)
    {
        std::shared_ptr<ResourcePile> ptr=std::shared_ptr<ResourcePile>(new ResourcePile(position));
        ptr->init(id, materialID,aItemDef);
        return ptr;
    }

    void ResourcePile::init(std::string id, std::string materialID, std::shared_ptr<properties::ItemDefinition> aItemDef)
    {
        StorageContainer::init(id,materialID,aItemDef);
    }
}