//
// Created by tatiana.polozova on 13.06.2018.
//

#include "BodySection.h"
#include "BodyPart.h"
#include "Body.h"
#include "GameManager.h"
#include "game/properties/GameDefinition.h"
#include "game/properties/BodySectionDef.h"

#include <algorithm>

namespace game
{
    BodySection::BodySection()
    {

    }

    BodySection::~BodySection()
    {

    }

    std::shared_ptr<BodySection> BodySection::create(unsigned int &key, std::shared_ptr<Body> parent, std::shared_ptr<properties::BodySectionDef> sectionDef, std::string nameModifier)
    {
        std::shared_ptr<BodySection> ptr=std::shared_ptr<BodySection>(new BodySection());
        ptr->init(key, parent, sectionDef, nameModifier);
        return ptr;
    }

    void BodySection::init(unsigned int &key, std::shared_ptr<Body> parent, std::shared_ptr<properties::BodySectionDef> sectionDef, std::string nameModifier)
    {
        ID=key;
        mHasGrip=false;

        parent_body=parent;

        mBodyPartID=sectionDef->BodyPartID;
        mBodyPart=BodyPart::create(shared_from_this(),GAME->gameDefinition()->bodyPartDefinition(mBodyPartID));

        mHeldItem= nullptr;

        mEquipType=sectionDef->EquipType;

        parent_body->addSection(shared_from_this());
        if (sectionDef->Hand)
        {
            if (nameModifier=="left")
                parent_body->addEquipmentSlot(shared_from_this(),EquipmentType::LeftHand);
            else
                parent_body->addEquipmentSlot(shared_from_this(),EquipmentType::RightHand);
        }

        for (auto connectedSection : sectionDef->ConnectedSections)
        {
            if (connectedSection->Symmetrical)
            {
                key++;
                mConnectsTo.push_back(BodySection::create(key,parent_body,connectedSection,"left"));
                key++;
                mConnectsTo.push_back(BodySection::create(key,parent_body,connectedSection,"right"));
            }
            else
            {
                key++;
                mConnectsTo.push_back(BodySection::create(key,parent_body,connectedSection,nameModifier));
            }
        }
    }

    void BodySection::addFunction(std::shared_ptr<BodyPart> bodyPart)
    {
        if (bodyPart->function()==BodyFunction::Grip)
            mHasGrip=true;

        parent_body->addFunction(bodyPart);
    }

    bool BodySection::hasStatus(BodySectionStatus status) const
    {
        auto iter=std::find_if(std::begin(mStatus),std::end(mStatus),[&status](BodySectionStatus const& value)
        {
            return status==value;
        });

        return (iter!=std::end(mStatus));
    }

    void BodySection::addStatus(BodySectionStatus status)
    {
        if (!this->hasStatus(status))
            mStatus.push_back(status);
    }

    void BodySection::removeStatus(game::BodySectionStatus status)
    {
        auto iter=std::find_if(std::begin(mStatus),std::end(mStatus),[&status](BodySectionStatus const& value)
        {
            return status==value;
        });

        if (iter!=std::end(mStatus))
            mStatus.erase(iter);
    }

    bool BodySection::canCarry() const
    {
        return mHasGrip && !this->hasStatus(BodySectionStatus::Missing) && !this->hasStatus(BodySectionStatus::Destroyed) && (mHeldItem==nullptr);
    }

    void BodySection::carryItem(std::shared_ptr<Item> item, bool alreadyCheck)
    {
        if (!alreadyCheck)
        {
            if (!this->canCarry())
                return;
        }

        mHeldItem=item;
    }

    const EquipmentType& BodySection::equipType() const
    {
        return mEquipType;
    }

    std::shared_ptr<Item> BodySection::heldItem() const
    {
        return mHeldItem;
    }

    void BodySection::dropItem()
    {
        mHeldItem=nullptr;
    }
}