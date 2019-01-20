//
// Created by tatiana on 06.01.19.
//

#include "BodyPartItem.h"
#include "BodySection.h"
#include "Character.h"
#include "Body.h"
#include "BodySection.h"

namespace game {
    BodyPartItem::BodyPartItem(const map::vector3& position)
        :StorageContainer(position)
    {

    }

    BodyPartItem::~BodyPartItem()
    {

    }

    std::shared_ptr<BodyPartItem> BodyPartItem::create(const map::vector3& position, std::shared_ptr<Character> character,
            std::shared_ptr<BodySection> limb, std::string id, std::shared_ptr<const properties::ItemDefinition> aItemDef)
    {
        std::shared_ptr<BodyPartItem> ptr=std::make_shared<BodyPartItem>(position);
        ptr->init(character,limb,id,aItemDef);
        return ptr;
    }

    void BodyPartItem::init(std::shared_ptr<Character> character, std::shared_ptr<BodySection> limb,
            std::string id, std::shared_ptr<const properties::ItemDefinition> aItemDef)
    {
        std::string materialID;
        if (limb!=nullptr)
            materialID=limb->materialID();
        else
            character->body()->bodySections().front()->materialID();

        StorageContainer::init(id,materialID,aItemDef);
    }
}