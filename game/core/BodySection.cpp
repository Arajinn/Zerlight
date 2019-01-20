//
// Created by tatiana on 13.06.2018.
//

#include "BodySection.h"
#include "defines.h"
#include "BodyPart.h"
#include "Body.h"
#include "GameManager.h"
#include "Attack.h"
#include "Item.h"
#include "ItemHistory.h"
#include "Character.h"
#include "Mind.h"
#include "Character.h"
#include "StringConstants.h"
#include "game/properties/GameDefinition.h"
#include "game/properties/BodySectionDef.h"
#include "game/properties/ItemDefinition.h"
#include "game/properties/DamageDef.h"
#include "game/properties/AttackDef.h"

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
        std::shared_ptr<BodySection> ptr=std::make_shared<BodySection>();
        ptr->init(key, parent, sectionDef, symmetricalType);
        return ptr;
    }

    void BodySection::init(unsigned int &key, std::shared_ptr<Body> parent, std::shared_ptr<properties::BodySectionDef> sectionDef,
                           BodySymmetricalType symmetricalType)
    {
        std::string nameModifier;
        if (symmetricalType==BodySymmetricalType::Left)
            nameModifier="left";
        else if (symmetricalType==BodySymmetricalType::Right)
            nameModifier="right";

        mName=nameModifier+((!nameModifier.empty()) ? " " : "")+sectionDef->Name;

        mID=key;
        mHasGrip=false;
        mToHitWeight=sectionDef->ToHitWeight;

        parent_body=parent;

        mBodyPartID=sectionDef->BodyPartID;
        mBodyPart=BodyPart::create(shared_from_this(),GAME_DEFINITIONS->bodyPartDefinition(mBodyPartID));
        mArtificialPart=nullptr;

        mHeldItem= nullptr;

        mEquipType=sectionDef->EquipType;
        mEquippedItem=nullptr;

        mLimb=sectionDef->Limb;
        mRemoveArtificialLimb=false;
        mBloodLossRate=0.0f;
        mConnection=sectionDef->Connection;

        if (sectionDef->TemplateMaterialIndex<0)
            mMaterialID=sectionDef->MaterialID;
        else
            mMaterialID=parent->character()->templateMaterialIDs().at((size_t)sectionDef->TemplateMaterialIndex);

        if (symmetricalType==BodySymmetricalType::Left)
        {
            if (!sectionDef->DecorationsLeft.empty())
                mSectionSpriteID=sectionDef->DecorationsLeft.at(0);
            else
                mSectionSpriteID = sectionDef->SpriteIDLeft;
        }
        else if (symmetricalType==BodySymmetricalType::Right)
        {
            if (!sectionDef->DecorationsRight.empty())
                mSectionSpriteID=sectionDef->DecorationsRight.at(0);
            else
                mSectionSpriteID = sectionDef->SpriteIDRight;
        }
        else
        {
            if (!sectionDef->Decorations.empty())
                mSectionSpriteID=sectionDef->Decorations.at(0);
            else
                mSectionSpriteID = sectionDef->SpriteID;
        }

        auto gender=parent_body->gender();
        if (gender==game::GenderType::Male)
        {
            for (const auto& decorations : sectionDef->AdditionalDecorationMale)
            {
                mAdditionalDecorations.push_back(decorations.at(0));
            }
        }
        else if (gender==game::GenderType::Female)
        {
            for (const auto& decorations : sectionDef->AdditionalDecorationFemale)
            {
                mAdditionalDecorations.push_back(decorations.at(0));
            }
        }
        else
        {
            for (const auto& decorations : sectionDef->AdditionalDecoration)
            {
                mAdditionalDecorations.push_back(decorations.at(0));
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

        for (const auto& connectedSection : sectionDef->ConnectedSections)
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

    void BodySection::regainFunction(std::shared_ptr<BodyPart> bodyPart)
    {
        if (bodyPart->function()==BodyFunction::Grip)
            mHasGrip=true;

        parent_body->regainFunction(bodyPart);
    }

    void BodySection::lostFunction(std::shared_ptr<BodyPart> bodyPart)
    {
        if (bodyPart->function()==BodyFunction::Grip)
            mHasGrip=false;

        parent_body->lostFunction(bodyPart);

        if ((bodyPart->function()!=BodyFunction::Structure) && (bodyPart->function()!=BodyFunction::Motor))
            return;

        for (const auto& section : mConnectsTo)
            section->bodyPart()->dependantPartLost(bodyPart);
    }

    void BodySection::damageFunction(std::shared_ptr<BodyPart> bodyPart)
    {
        parent_body->damageFunction(bodyPart);

        if ((bodyPart->function()!=BodyFunction::Structure) && (bodyPart->function()!=BodyFunction::Motor))
            return;

        for (const auto& section : mConnectsTo)
            section->bodyPart()->dependantPartDamaged(bodyPart);
    }

    bool BodySection::hasStatus(const BodySectionStatus& status) const
    {
        auto iter=std::find_if(mStatus.begin(),mStatus.end(),[&status](BodySectionStatus const& value)
        {
            return status==value;
        });

        return (iter!=mStatus.end());
    }

    void BodySection::addStatus(const BodySectionStatus& status)
    {
        if (!hasStatus(status))
            mStatus.push_back(status);
    }

    void BodySection::removeStatus(const BodySectionStatus& status)
    {
        auto iter=std::find_if(mStatus.begin(),mStatus.end(),[&status](BodySectionStatus const& value)
        {
            return status==value;
        });

        if (iter!=mStatus.end())
            mStatus.erase(iter);
    }

    bool BodySection::canCarry() const
    {
        return mHasGrip && !hasStatus(BodySectionStatus::Missing) && !hasStatus(BodySectionStatus::Destroyed) && (mHeldItem==nullptr);
    }

    void BodySection::carryItem(std::shared_ptr<Item> item, bool alreadyCheck)
    {
        if (!alreadyCheck)
        {
            if (!canCarry())
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

        for (size_t i=0,isize=mAdditionalDecorations.size();i<isize;i++)
        {
            properties::TileDef info;
            info.DrawOrder=float(mDrawOrder+float(i)*0.01);
            info.Color=gui::ZColor(-1,-1,-1);
            info.SpriteID=mAdditionalDecorations.at(i);
            tiles.push_back(info);
        }

        for (const auto& connectedSection : mConnectsTo)
            connectedSection->getTiles(tiles);
    }

    std::shared_ptr<Item> BodySection::equippedItem() const
    {
        return mEquippedItem;
    }

    unsigned int BodySection::ID() const
    {
        return mID;
    }

    const float& BodySection::toHitWeight() const
    {
        return mToHitWeight;
    }

    bool BodySection::hasFunction(const BodyFunction& function) const
    {
        if ((hasStatus(BodySectionStatus::Missing)) || (hasStatus(BodySectionStatus::Destroyed)))
            return false;

        return mBodyPart->hasFunction(function);
    }

    bool BodySection::directlyConnectsToFunction(const BodyFunction& function) const
    {
        if ((hasStatus(BodySectionStatus::Missing)) || (hasStatus(BodySectionStatus::Destroyed)))
            return false;

        for (const auto& section : mConnectsTo)
        {
            if (section->hasFunction(function))
                return true;
        }

        return false;
    }

    std::shared_ptr<BodyPart> BodySection::bodyPart() const
    {
        if (hasStatus(BodySectionStatus::Artificial))
            return mArtificialPart;

        return mBodyPart;
    }

    float BodySection::equipmentQualityModifier() const
    {
        if (mEquippedItem==nullptr)
            return 1.0f;

        const auto quality=mEquippedItem->quality();
        switch (quality)
        {
            case game::ItemQuality::Poor:
                return 0.9f;
            case game::ItemQuality::Average:
                return 1.0f;
            case game::ItemQuality::Fine:
                return 1.1f;
            case game::ItemQuality::Superior:
                return 1.2f;
            case game::ItemQuality::Masterful:
                return 1.4f;
            case game::ItemQuality::Legendary:
                return 1.8f;
            default:
                return 1.0f;
        }
    }

    std::shared_ptr<Body> BodySection::body() const
    {
        return parent_body;
    }

    void BodySection::addWeapon(std::shared_ptr<Weapon> weapon)
    {
        parent_body->addNaturalWeapon(weapon);
    }

    void BodySection::removeWeapon(std::shared_ptr<Weapon> weapon)
    {
        parent_body->removeNaturalWeapon(weapon);
    }

    void BodySection::unequip(std::shared_ptr<Item> item)
    {
        if (mEquippedItem->ID()!=item->ID())
            return;

        mEquippedItem=nullptr;

        for (const auto& section : mConnectsTo)
        {
            if (section->equipType()==mEquipType)
                section->unequip(item);
        }
    }

    void BodySection::damageEquipment(std::shared_ptr<Attack> attack, size_t damageTypeIndex, std::vector<std::string>& hitEffects)
    {
        const auto damage=attack->damage(damageTypeIndex);
        const auto damageType=damage.Type;
        const auto damageValue=damage.Amount;

        const auto equipMaterialDef=GAME_DEFINITIONS->materialDefinition(mEquippedItem->materialID());
        const auto damageDef=equipMaterialDef->DamageProperties.at((size_t)damageType);
        float value=equipMaterialDef->Sustains*mEquippedItem->itemDef()->Thickness*equipmentQualityModifier();

        if (mEquippedItem->damageTaken()>=value)
        {
            bodyPart()->damage(attack,damageTypeIndex,hitEffects);
        }
        else if (damageValue>=damageDef->Blocks)
        {
            const float delta=damageValue-damageDef->Blocks;
            const float getDamage=delta*damageDef->PercentReceived;
            mEquippedItem->setDamageTaken(mEquippedItem->damageTaken()+getDamage);
            if (getDamage>=damageDef->Break)
                mEquippedItem->setDamageTaken(value+getDamage);

            if (mEquippedItem->damageTaken()>=value)
            {
                hitEffects.push_back(properties::AttackDef::damageString(attack->damage(damageTypeIndex).Type)+" the "+mEquippedItem->name());
                attack->setDamage(damageTypeIndex,mEquippedItem->damageTaken()-value);
                bodyPart()->damage(attack,damageTypeIndex,hitEffects);
                parent_body->unequip(mEquippedItem);
                mEquippedItem->toDeconstruct();
            }
            else
            {
                bodyPart()->damageFunctionThroughArmor(delta);

                hitEffects.push_back(properties::AttackDef::damageString(attack->damage(damageTypeIndex).Type)+" the "+mEquippedItem->name());

                if (damageDef->PercentTransfered>0.0f)
                {
                    attack->setDamage(damageTypeIndex,delta*damageDef->PercentTransfered);
                    bodyPart()->damage(attack,damageTypeIndex,hitEffects);
                }
                else
                {
                    damageTypeIndex++;
                    if (damageTypeIndex>=attack->damageCount())
                        return;

                    damageEquipment(attack,damageTypeIndex,hitEffects);
                }
            }
        }
        else
        {
            damageTypeIndex++;
            if (damageTypeIndex<attack->damageCount())
                damageEquipment(attack,damageTypeIndex,hitEffects);
            else
                hitEffects.push_back("stopping at the "+mEquippedItem->name());
        }
    }

    void BodySection::damage(std::shared_ptr<Attack> attack, std::vector<std::string>& hitEffects)
    {
        hitEffects.push_back("hitting the "+mName);

        std::cout << "hitting the " + mName << std::endl;

        if (mEquippedItem!=nullptr)
            damageEquipment(attack,0,hitEffects);
        else
            bodyPart()->damage(attack,0,hitEffects);

        checkStatus();
    }

    void BodySection::setMissing()
    {
        addStatus(BodySectionStatus::Missing);

        parent_body->character()->mind()->adjustHappiness(-50.0f);
        bodyPart()->disable();

        if (mEquipType!=EquipmentType::None)
            parent_body->remove(mEquippedItem);

        for (const auto& section : mConnectsTo)
            section->setMissing();
    }

    std::string BodySection::bodyPartID() const
    {
        return mBodyPartID;
    }

    std::shared_ptr<Item> BodySection::removeArtificialPart()
    {
        if (mArtificialPart==nullptr)
            return nullptr;

        auto item=mArtificialPart->removeProstheticPart();

        removeStatus(BodySectionStatus::Artificial);
        addStatus(BodySectionStatus::Missing);

        mArtificialPart->pre_delete();
        mArtificialPart=nullptr;
        mRemoveArtificialLimb=false;

        if (mHeldItem!=nullptr)
            parent_body->character()->dropItem(mHeldItem);

        for (const auto& section : mConnectsTo)
        {
            if (ITEM_SETTINGS->bodyPartToItemID(section->bodyPartID())!=ITEMID_NONE)
            {
                const auto components=item->history()->components();
                for (const auto& component : components)
                {
                    if (component->ID()==section->bodyPart()->prostheticPart()->ID())
                    {
                        section->removeArtificialPart();
                        break;
                    }
                }
            }

        }

        return item;
    }

    void BodySection::checkStatus()
    {
        if (mLimb && (bodyPart()->isSevered()) && (!hasStatus(BodySectionStatus::Missing)))
        {
            setMissing();

            if (bodyPart()->prostheticPart()!=nullptr)
                parent_body->removeArtificialLimb(bodyPart()->prostheticPart());
        }

        if (bodyPart()->isBleeding())
            addStatus(BodySectionStatus::Bleeding);

        if (bodyPart()->hasInternalBleeding())
            addStatus(BodySectionStatus::InternalBleeding);

        if (bodyPart()->isArteryStruck())
            addStatus(BodySectionStatus::StruckArtery);

        if ((bodyPart()->isCompletelyDestroyed()) && (!hasStatus(BodySectionStatus::Missing)))
        {
            addStatus(BodySectionStatus::Destroyed);
            bodyPart()->disable();
            parent_body->character()->mind()->adjustHappiness(-50.0f);
            if (bodyPart()->prostheticPart()!=nullptr)
                parent_body->removeArtificialLimb(bodyPart()->prostheticPart());
        }

        mBloodLossRate=0.200000002980232f*float(bodyPart()->bleedingCount())+1.0f*float(bodyPart()->damagedArteriesCount());
    }

    float BodySection::bloodLossRate() const
    {
        return mBloodLossRate;
    }

    std::vector<BodySectionStatus> BodySection::status() const
    {
        return mStatus;
    }

    std::string BodySection::name() const
    {
        return mName;
    }

    bool BodySection::isConnection() const
    {
        return mConnection;
    }

    std::vector<std::shared_ptr<BodySection>> BodySection::connectsTo() const
    {
        return mConnectsTo;
    }

    void BodySection::repairEquipment()
    {
        if (mArtificialPart!=nullptr)
            mArtificialPart->healWound();

        if (mEquippedItem!=nullptr)
            mEquippedItem->repair();
    }

    std::string BodySection::materialID() const
    {
        return mMaterialID;
    }
}