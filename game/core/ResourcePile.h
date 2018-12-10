//
// Created by tatiana on 22.06.2018.
//

#ifndef ZERL_RESOURCEPILE_H
#define ZERL_RESOURCEPILE_H

#include "game/core/StorageContainer.h"

namespace game{
    class ResourcePile : public StorageContainer
    {
    public:
        ResourcePile(const map::vector3& position);
        ~ResourcePile();

        static std::shared_ptr<ResourcePile> create(const map::vector3& position, std::string id, std::string materialID, std::shared_ptr<properties::ItemDefinition> aItemDef);

    protected:
        virtual void init(std::string id, std::string materialID, std::shared_ptr<properties::ItemDefinition> aItemDef);

    };
}

#endif //ZERL_RESOURCEPILE_H
