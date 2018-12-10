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
        ~StorageContainer();

        static std::shared_ptr<StorageContainer> create(const map::vector3& position, std::string id, std::string materialID, std::shared_ptr<properties::ItemDefinition> aItemDef);

    protected:
        virtual void init(std::string id, std::string materialID, std::shared_ptr<properties::ItemDefinition> aItemDef);
    };
}

#endif //ZERL_STORAGECONTAINER_H
