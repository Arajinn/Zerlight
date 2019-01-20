//
// Created by tatiana on 06.01.19.
//

#ifndef ZERLIGHT_BODYPARTITEM_H
#define ZERLIGHT_BODYPARTITEM_H

#include "game/core/StorageContainer.h"

namespace game
{
    class Character;
    class BodySection;
}

namespace game {
    class BodyPartItem : public StorageContainer{
    public:
        BodyPartItem(const map::vector3& position);
        virtual ~BodyPartItem();

        static std::shared_ptr<BodyPartItem> create(const map::vector3& position, std::shared_ptr<Character> character,
                std::shared_ptr<BodySection> limb, std::string id, std::shared_ptr<const properties::ItemDefinition> aItemDef);

    protected:
        virtual void init(std::shared_ptr<Character> character, std::shared_ptr<BodySection> limb,
                std::string id, std::shared_ptr<const properties::ItemDefinition> aItemDef);
    };
}

#endif //ZERLIGHT_BODYPARTITEM_H
