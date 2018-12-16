//
// Created by tatiana on 13.06.2018.
//

#include "BodySection.h"
#include "defines.h"
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

    std::shared_ptr<BodySection> BodySection::create(unsigned int &key, std::shared_ptr<Body> parent,
            std::shared_ptr<properties::BodySectionDef> sectionDef, BodySymmetricalType symmetricalType)
    {
        std::shared_ptr<BodySection> ptr=std::shared_ptr<BodySection>(new BodySection());
        ptr->init(key, parent, sectionDef, symmetricalType);
        return ptr;
    }

    void BodySection::init(unsigned int &key, std::shared_ptr<Body> parent, std::shared_ptr<properties::BodySectionDef> sectionDef,
                           BodySymmetricalType symmetricalType)
    {
        ID=key;
        mHasGrip=false;

        parent_body=parent;

        mBodyPartID=sectionDef->BodyPartID;
        mBodyPart=BodyPart::create(shared_from_this(),GAME_DEFINITIONS->bodyPartDefinition(mBodyPartID));

        mHeldItem= nullptr;

        mEquipType=sectionDef->EquipType;

        if (symmetricalType==BodySymmetricalType::Left)
        {
            if (sectionDef->DecorationsLeft.size()>0)
                mSectionSpriteID=sectionDef->DecorationsLeft.at(0);
            else
                mSectionSpriteID = sectionDef->SpriteIDLeft;
        }
        else if (symmetricalType==BodySymmetricalType::Right)
        {
            if (sectionDef->DecorationsRight.size()>0)
                mSectionSpriteID=sectionDef->DecorationsRight.at(0);
            else
                mSectionSpriteID = sectionDef->SpriteIDRight;
        }
        else
        {
            if (sectionDef->Decorations.size()>0)
                mSectionSpriteID=sectionDef->Decorations.at(0);
            else
                mSectionSpriteID = sectionDef->SpriteID;
        }

        auto gender=parent_body->gender();
        if (gender==game::GenderType::Male)
        {
            for (int i=0,isize=sectionDef->AdditionalDecorationMale.size();i<isize;i++)
            {
                mAdditionalDecorations.push_back(sectionDef->AdditionalDecorationMale.at(i).at(0));
            }
        }
        else if (gender==game::GenderType::Female)
        {
            for (int i=0,isize=sectionDef->AdditionalDecorationFemale.size();i<isize;i++)
            {
                mAdditionalDecorations.push_back(sectionDef->AdditionalDecorationFemale.at(i).at(0));
            }
        }
        else
        {
            for (int i=0,isize=sectionDef->AdditionalDecoration.size();i<isize;i++)
            {
                mAdditionalDecorations.push_back(sectionDef->AdditionalDecoration.at(i).at(0));
            }
        }

        mDrawOrder=sectionDef->DrawOrder;

        mSymmetricalType=symmetricalType;

        parent_body->addSection(shared_from_this());
        if (sectionDef->Hand)
        {
            if (symmetricalType==BodySymmetricalType::Left)
                parent_body->addEquipmentSlot(shared_from_this(),EquipmentType::LeftHand);
            else
                parent_body->addEquipmentSlot(shared_from_this(),EquipmentType::RightHand);
        }

        for (auto connectedSection : sectionDef->ConnectedSections)
        {
            if (connectedSection->Symmetrical)
            {
                key++;
                mConnectsTo.push_back(BodySection::create(key,parent_body,connectedSection,BodySymmetricalType::Left));
                key++;
                mConnectsTo.push_back(BodySection::create(key,parent_body,connectedSection,BodySymmetricalType::Right));
            }
            else
            {
                key++;
                mConnectsTo.push_back(BodySection::create(key,parent_body,connectedSection,symmetricalType));
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

    void BodySection::getTiles(std::vector<properties::TileDef>& tiles) const
    {
        if (hasStatus(BodySectionStatus::Missing))
            return;

        if (!mSectionSpriteID.empty())
        {
            properties::TileDef info;
            info.DrawOrder=float(mDrawOrder);
            info.Color=gui::ZColor(-1,-1,-1);
            info.SpriteID=mSectionSpriteID;
            tiles.push_back(info);
        }

        for (int i=0,isize=mAdditionalDecorations.size();i<isize;i++)
        {
            properties::TileDef info;
            info.DrawOrder=float(mDrawOrder+float(i)*0.01);
            info.Color=gui::ZColor(-1,-1,-1);
            info.SpriteID=mAdditionalDecorations.at(i);
            tiles.push_back(info);
        }

        for (const auto connectedSection : mConnectsTo)
            connectedSection->getTiles(tiles);
    }
}