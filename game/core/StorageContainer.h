//
// Created by tatiana on 22.06.2018.
//

#ifndef ZERL_STORAGECONTAINER_H
#define ZERL_STORAGECONTAINER_H

#include "game/core/Item.h"

namespace game {
    class StorageContainer : public Item
    {
    public:
        StorageContainer(const map::vector3& position);
        virtual ~StorageContainer();

        static std::shared_ptr<StorageContainer> create(const map::vector3& position, std::string id, std::string materialID,
                std::shared_ptr<const properties::ItemDefinition> aItemDef);

        std::vector<std::shared_ptr<Item>> containedResources() const;
        int containedResourcesCount() const;

        void removeItem(std::shared_ptr<Item> item);
    protected:
        virtual void init(std::string id, std::string materialID, std::shared_ptr<const properties::ItemDefinition> aItemDef);

        std::vector<std::shared_ptr<Item>> mContainedResources;
    };
}

#endif //ZERL_STORAGECONTAINER_H
