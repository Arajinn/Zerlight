//
// Created by tatiana on 6/14/2018.
//

#include "BodyPart.h"
#include "BodySection.h"
#include "game/properties/BodyPartDef.h"

namespace game
{
    BodyPart::BodyPart()
    {

    }

    BodyPart::~BodyPart()
    {

    }

    std::shared_ptr<BodyPart> BodyPart::create(std::shared_ptr<BodySection> section,
            std::shared_ptr<const properties::BodyPartDef> aBodyPartDef,
            std::string nameModifier, std::shared_ptr<BodyPart> containedBy, std::shared_ptr<Item> prostheticPart)
    {
        std::shared_ptr<BodyPart> ptr=std::shared_ptr<BodyPart>(new BodyPart());
        ptr->init(section,aBodyPartDef,nameModifier,containedBy,prostheticPart);
        return ptr;
    }

    void BodyPart::init(std::shared_ptr<game::BodySection> section,std::shared_ptr<const properties::BodyPartDef> aBodyPartDef,
            std::string nameModifier, std::shared_ptr<game::BodyPart> containedBy,std::shared_ptr<game::Item> prostheticPart)
    {
        parent_section=section;
        parent_contained=containedBy;
        mProstheticPart=prostheticPart;
        mBodyFunction=aBodyPartDef->BodyFunction;

        if (mProstheticPart==nullptr)
            this->addFunction();

        mBodyPartDef=aBodyPartDef;
        for (auto containedPartDef : aBodyPartDef->ContainedParts)
        {
            if (containedPartDef->Symmetrical)
            {
                mContainedParts.push_back(BodyPart::create(parent_section,containedPartDef,"left",shared_from_this(),mProstheticPart));
                mContainedParts.push_back(BodyPart::create(parent_section,containedPartDef,"right",shared_from_this(),mProstheticPart));
            }
            else
            {
                mContainedParts.push_back(BodyPart::create(parent_section,containedPartDef,nameModifier,shared_from_this(),mProstheticPart));
            }
        }

        mStatus=BodyPartStatus::Good;
    }

    const BodyFunction& BodyPart::function() const
    {
        return mBodyFunction;
    }

    void BodyPart::addFunction()
    {
        parent_section->addFunction(shared_from_this());
    }
}