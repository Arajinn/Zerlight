//
// Created by tatiana on 13.06.2018.
//

#ifndef ZERL_BODYSECTION_H
#define ZERL_BODYSECTION_H

#include "game/core/enums.h"
#include "game/properties/TileDef.h"

#include <vector>
#include <memory>

namespace game
{
    class Body;
    class BodyPart;
    class Item;
}

namespace properties
{
    class BodySectionDef;
}

namespace game {
class BodySection : public std::enable_shared_from_this<BodySection> {
    public:
        BodySection();
        ~BodySection();

        static std::shared_ptr<BodySection> create(unsigned int &key, std::shared_ptr<Body> parent,
                std::shared_ptr<properties::BodySectionDef> sectionDef, BodySymmetricalType symmetricalType=BodySymmetricalType::None);

        void addFunction(std::shared_ptr<BodyPart> bodyPart);

        bool canCarry() const;
        void carryItem(std::shared_ptr<Item> item, bool alreadyCheck=false);

        std::shared_ptr<Item> heldItem() const;

        void dropItem();

        const EquipmentType& equipType() const;

        void getTiles(std::vector<properties::TileDef>& tiles) const;
    private:
        friend class Body;
        void init(unsigned int &key, std::shared_ptr<Body> parent, std::shared_ptr<properties::BodySectionDef> sectionDef,
                BodySymmetricalType symmetricalType);

        unsigned int ID;

        std::shared_ptr<Body> parent_body;
        std::string mBodyPartID;
        std::shared_ptr<BodyPart> mBodyPart;
        std::vector<std::shared_ptr<BodySection>> mConnectsTo;
        bool mHasGrip;
        std::shared_ptr<Item> mHeldItem;
        EquipmentType mEquipType;
        std::string mSectionSpriteID;
        std::vector<std::string> mAdditionalDecorations;
        int mDrawOrder;
        BodySymmetricalType mSymmetricalType;

        std::vector<BodySectionStatus> mStatus;
        bool hasStatus(BodySectionStatus status) const;
        void addStatus(BodySectionStatus status);
        void removeStatus(BodySectionStatus status);
    };
}

#endif //ZERL_BODYSECTION_H
