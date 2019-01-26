//
// Created by tatiana on 22.06.2018.
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

    std::shared_ptr<ResourcePile> ResourcePile::create(const map::vector3& position, const std::string& id, const MaterialID_t& materialID, std::shared_ptr<const properties::ItemDefinition> aItemDef)
    {
        auto ptr=std::make_shared<ResourcePile>(position);
        ptr->init(id, materialID,aItemDef);
        return ptr;
    }

    void ResourcePile::init(const std::string& id, const MaterialID_t& materialID, std::shared_ptr<const properties::ItemDefinition> aItemDef)
    {
        StorageContainer::init(id,materialID,aItemDef);
    }
}