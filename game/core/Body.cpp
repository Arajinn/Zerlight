//
// Created by tatiana on 26.03.2018.
//

#include "Body.h"
#include "defines.h"
#include "Character.h"
#include "CharacterHistory.h"
#include "Automaton.h"
#include "HealthStatusEffect.h"
#include "SquadPosition.h"
#include "BodySection.h"
#include "BodyPart.h"
#include "Mind.h"
#include "GameManager.h"
#include "Item.h"
#include "Weapon.h"
#include "StorageContainer.h"
#include "StringConstants.h"
#include "game/properties/RaceDefinition.h"
#include "game/properties/GameDefinition.h"
#include "game/properties/BodyDef.h"
#include "game/properties/WeaponDef.h"
#include "game/properties/ItemDefinition.h"
#include "game/properties/ItemSettings.h"
#include "game/properties/CharacterSettings.h"
#include "game/map/MapCell.h"
#include "game/utils/math_utils.h"
#include "game/utils/Randomizer.h"

#include <algorithm>
#include <iostream>

namespace game
{
    Body::Body()
    {

    }

    Body::~Body()
    {

    }

    std::shared_ptr<Body> Body::create(std::shared_ptr<game::Character> parent)
    {
        std::shared_ptr<Body> ptr=std::make_shared<Body>();
        ptr->init(std::move(parent));
        return ptr;
    }

    void Body::init(std::shared_ptr<game::Character> parent)
    {
        owner=parent;

        auto raceDef=parent->raceDefinition();

        mMovementPenalty=0.0f;
        mJobPenalty=0.0f;

        mIsDead=false;

        mIsBleeding=false;
        mBloodLevel=100.0f;
        mBloodLossRate=0.0f;
        mSpawnBloodTimer=0.0f;

        mIsSleeping=false;
        mRestLevel=100.0f;
        mRestRate=0.0f;

        mThirstLevel=100.0f;
        mLookingForDrink=false;

        mHungerLevel=50.0f;
        mLookingForFood=false;

        mGestationTime=0.0f;
        mReadyToMate=false;

        mCombatValue=0.0f;

        mLungCapacity=100.0f;

        mPickupWeaponTimer=0.0f;

        mDodgeTimer=0.0f;

        mBodyFunctionTotal.resize((size_t)BodyFunction::Count);
        std::fill(mBodyFunctionTotal.begin(),mBodyFunctionTotal.end(),0);
        mBodyFunctionCurrent.resize((size_t)BodyFunction::Count);
        std::fill(mBodyFunctionCurrent.begin(),mBodyFunctionCurrent.end(),0);
        mDamagedFunction.resize((size_t)BodyFunction::Count);
        std::fill(mDamagedFunction.begin(),mDamagedFunction.end(),false);
        mLostFunction.resize((size_t)BodyFunction::Count);
        std::fill(mLostFunction.begin(),mLostFunction.end(),false);

        auto bodyDef=GAME_DEFINITIONS->bodyDefinition(raceDef->BodyID);
        bodySectionsKey=0;
        mMainBodySection=BodySection::create(bodySectionsKey,shared_from_this(),bodyDef->MainBody,BodySymmetricalType::None);
    }

    float Body::exhaustionModifier() const
    {
        return math_utils::clamp(mRestLevel/owner->raceDefinition()->TiredLevel,0.5f,1.0f);
    }

    bool Body::containStatusEffect(const HealthStatusAilment& key) const
    {
        auto iter=std::find_if(mStatusEffects.begin(),mStatusEffects.end(),[&key](HealthStatusEffect const& value)
        {
            return (value.Ailment==key);
        });

        return (iter!=mStatusEffects.end());
    }

    void Body::removeStatusEffect(const HealthStatusAilment& key)
    {
        auto iter=std::find_if(mStatusEffects.begin(),mStatusEffects.end(),[&key](HealthStatusEffect const& value)
        {
            return (value.Ailment==key);
        });

        if (iter!=mStatusEffects.end())
            mStatusEffects.erase(iter);
    }

    void Body::addStatusEffect(const HealthStatusAilment& key, const float& amount, const float& recoveryRate, const bool& effectRecoveryRate)
    {
        auto iter=std::find_if(mStatusEffects.begin(),mStatusEffects.end(),[&key](HealthStatusEffect const& value)
        {
            return (value.Ailment==key);
        });

        if (iter==mStatusEffects.end())
        {
            mStatusEffects.emplace_back(HealthStatusEffect(key,amount,effectRecoveryRate));
        }
        else
        {
            iter->Amount+=amount;

            if (!effectRecoveryRate)
                return;

            iter->RecoveryRate=effectRecoveryRate;
        }
    }

    float Body::movementPenalty() const
    {
        return (1.0f-mMovementPenalty*0.00999999977648258f);
    }

    float Body::moveSpeed() const
    {
        if (mIsSleeping)
            return 0.0f;

        float value=owner->raceDefinition()->MoveSpeed*exhaustionModifier()*owner->attributeLevel(CharacterAttributeType::Nimbleness);

        if (containStatusEffect(HealthStatusAilment::FallenOver))
            value=value*0.1f;

        value=value*movementPenalty();

        auto squadPosition=owner->squadPosition();
        if (squadPosition!= nullptr)
        {
            if (squadPosition->perk()==SquadPositionPerk::Scout)
                value=value*1.2f;
            else if ((squadPosition->perk()==SquadPositionPerk::SurvivalInstinct) && mIsBleeding)
                value=value*1.4f;
        }

        return value;
    }

    const bool& Body::isDead() const
    {
        return mIsDead;
    }

    bool Body::isThirsty() const
    {
        return (mThirstLevel<owner->raceDefinition()->ThirstLevel);
    }

    bool Body::isDyingOfThirst() const
    {
        return (mThirstLevel<owner->raceDefinition()->DyingOfThirstLevel);
    }

    const bool& Body::isLookingForDrink() const
    {
        return mLookingForDrink;
    }

    void Body::setLookingForDrink()
    {
        mLookingForDrink=true;
    }

    void Body::addEquipmentSlot(std::shared_ptr<BodySection> section, EquipmentType equipType)
    {
        auto iter=std::find_if(mEquipmentSlots.begin(),mEquipmentSlots.end(),[&equipType](BodySectionInfo const& value)
        {
            return equipType==value.type;
        });

        if (iter==mEquipmentSlots.end())
        {
            BodySectionInfo newInfo;
            newInfo.type=equipType;
            newInfo.sections.push_back(section);
            mEquipmentSlots.push_back(newInfo);
        }
        else
        {
            auto iter_section=std::find_if(iter->sections.begin(),iter->sections.end(),[&section](std::shared_ptr<BodySection> const& value)
            {
                return section->ID()==value->ID();
            });

            if (iter_section==iter->sections.end())
                return;
            else
            {
                const auto index=std::distance(mEquipmentSlots.begin(),iter);
                mEquipmentSlots[index].sections.push_back(section);
            }
        }
    }

    void Body::addFunction(std::shared_ptr<BodyPart> bodyPart)
    {
        auto function=bodyPart->function();
        if (function==BodyFunction::None)
            return;

        mBodyFunctionTotal[(size_t)function]+=1;
        mBodyFunctionCurrent[(size_t)function]+=1;
    }

    void Body::regainFunction(std::shared_ptr<BodyPart> bodyPart)
    {
        auto function=bodyPart->function();
        if (function==BodyFunction::None)
            return;

        mBodyFunctionCurrent[(size_t)function]+=1;

        if ((function!=BodyFunction::Stand) || (abilityLevel(BodyFunction::Stand)<=0.5f))
            return;

        removeStatusEffect(HealthStatusAilment::FallenOver);
    }

    void Body::lostFunction(std::shared_ptr<BodyPart> bodyPart)
    {
        auto function=bodyPart->function();
        if (function==BodyFunction::None)
            return;

        if (function==BodyFunction::Grip)
        {
            const auto heldItem=bodyPart->section()->heldItem();
            if (heldItem!=nullptr)
            {
                auto iter=std::find_if(mItemsToDrop.begin(),mItemsToDrop.end(),[&heldItem](std::shared_ptr<Item> const& elem)
                {
                    return heldItem->ID()==elem->ID();
                });

                if (iter==mItemsToDrop.end())
                    mItemsToDrop.push_back(heldItem);
            }
        }

        mLostFunction[(size_t)function]=true;

        mBodyFunctionCurrent[(size_t)function]-=1;
        if (mBodyFunctionCurrent[(size_t)function]>0)
            return;

        completelyLostFunction(bodyPart);
    }

    void Body::completelyLostFunction(std::shared_ptr<BodyPart> bodyPart)
    {
        auto function=bodyPart->function();

        if ((function==BodyFunction::Thought) || (function==BodyFunction::Circulation)
            || (function==BodyFunction::Breathe) || (function==BodyFunction::Throat))
            mIsDead=true;
    }

    void Body::damageFunction(std::shared_ptr<BodyPart> bodyPart)
    {
        auto function=bodyPart->function();
        if (function==BodyFunction::None)
            return;

        if (function==BodyFunction::Grip)
        {
            const auto heldItem=bodyPart->section()->heldItem();
            if (heldItem!=nullptr)
            {
                auto iter=std::find_if(mItemsToDrop.begin(),mItemsToDrop.end(),[&heldItem](std::shared_ptr<Item> const& elem)
                {
                    return heldItem->ID()==elem->ID();
                });

                if (iter==mItemsToDrop.end())
                {
                    const float rand=RANDOMIZER->uniform(0.0f,1.0f);
                    const auto itemID=bodyPart->section()->heldItem()->itemID();
                    const auto itemDef=GAME_DEFINITIONS->itemDefinition(itemID);
                    const float skillModifier=owner->history()->skillModifier(itemDef->ItemWeaponDef->Skill);
                    const float fitness=owner->attributeLevel(CharacterAttributeType::Fitness);
                    if (rand>(0.5f+skillModifier)*fitness)
                    {
                        mItemsToDrop.push_back(heldItem);
                    }
                }
            }
        }

        mDamagedFunction[(size_t)function]=true;
    }

    bool Body::pickupItem(std::shared_ptr<Item> item, std::vector<std::shared_ptr<BodySection>> sections)
    {
        for (const auto& section : sections)
        {
            if (section->canCarry())
            {
                section->carryItem(item,true);

                auto iter=std::find_if(mHeldItems.begin(),mHeldItems.end(),[&item](std::shared_ptr<Item> const& value)
                {
                    return item->ID()==value->ID();
                });

                if (iter==mHeldItems.end())
                    mHeldItems.push_back(item);

                return true;
            }
        }

        return false;
    }

    bool Body::isEqupmentSlotsContain(EquipmentType equipType) const
    {
        auto iter=std::find_if(mEquipmentSlots.begin(),mEquipmentSlots.end(),[&equipType](BodySectionInfo const& value)
        {
            return equipType==value.type;
        });

        return (iter!=mEquipmentSlots.end());
    }

    bool Body::pickupItem(std::shared_ptr<Item> item, EquipmentType equipType)
    {
        auto iter=std::find_if(mEquipmentSlots.begin(),mEquipmentSlots.end(),[&equipType](BodySectionInfo const& value)
        {
            return equipType==value.type;
        });

        if (iter==mEquipmentSlots.end())
            return false;

        pickupItem(item,iter->sections);
    }

    bool Body::pickupItem(std::shared_ptr<Item> item)
    {
        if (pickupItem(item,EquipmentType::LeftHand))
            return true;

        return pickupItem(item,EquipmentType::RightHand);
    }

    float Body::abilityLevel(BodyFunction bodyFunction)
    {
        if (mBodyFunctionTotal.at((size_t)bodyFunction)!=0)
            return float(mBodyFunctionCurrent.at((size_t)bodyFunction))/float(mBodyFunctionTotal.at((size_t)bodyFunction));

        return (bodyFunction==BodyFunction::Wing) ? 0.0f : 1.0f;
    }

    void Body::addEquipmentSlot(std::shared_ptr<BodySection> section)
    {
        if (section->equipType()==EquipmentType::None)
            return;

        addEquipmentSlot(section,section->equipType());
    }

    void Body::addSection(std::shared_ptr<BodySection> section)
    {
        auto iter=std::find_if(mBodySections.begin(),mBodySections.end(),[&section](std::shared_ptr<BodySection> const& value)
        {
            return section->ID()==value->ID();
        });

        if (iter==mBodySections.end())
        {
            mBodySections.push_back(section);
            addEquipmentSlot(section);
        }
    }

    bool Body::dropItem(std::shared_ptr<Item> item, std::vector<std::shared_ptr<BodySection>> sections)
    {
        if (sections.empty())
            return false;

        for (const auto& section : sections)
        {
            if (section->heldItem()->ID()==item->ID())
            {
                section->dropItem();
                return true;
            }
        }

        return false;
    }

    bool Body::dropItem(std::shared_ptr<Item> item)
    {
        auto iter=std::find_if(mHeldItems.begin(),mHeldItems.end(),[&item](std::shared_ptr<Item> const& value)
        {
            return item->ID()==value->ID();
        });

        if (iter==mHeldItems.end())
            return false;

        auto iter_equip_left=std::find_if(mEquipmentSlots.begin(),mEquipmentSlots.end(),[](BodySectionInfo const& value)
        {
            return value.type==EquipmentType::LeftHand;
        });

        if (iter_equip_left!=mEquipmentSlots.end())
        {
            if (dropItem(item,iter_equip_left->sections))
            {
                mHeldItems.erase(iter);
                return true;
            }
        }

        auto iter_equip_right=std::find_if(mEquipmentSlots.begin(),mEquipmentSlots.end(),[](BodySectionInfo const& value)
        {
            return value.type==EquipmentType::RightHand;
        });

        if (iter_equip_right!=mEquipmentSlots.end())
        {
            if (dropItem(item,iter_equip_right->sections))
            {
                mHeldItems.erase(iter);
                return true;
            }
        }

        return false;
    }

    void Body::drinkItem(std::shared_ptr<Item> item)
    {
        mThirstLevel+=item->effectAmount(ItemEffectType::Drink)*owner->raceDefinition()->DrinkRatio;
        dropItem(item);

        if (mThirstLevel>100.0f)
        {
            mLookingForDrink=false;
        }
    }

    bool Body::isStarving() const
    {
        return (mHungerLevel<owner->raceDefinition()->StarvationLevel);
    }

    bool Body::isHungry() const
    {
        return (mHungerLevel<owner->raceDefinition()->HungerLevel);
    }

    bool Body::update(float dt)
    {
        if (mIsDead)
            return false;

        auto raceDef=owner->raceDefinition();
        float fitness=owner->attributeLevel(CharacterAttributeType::Fitness);

        mBloodLevel-=mBloodLossRate*raceDef->BloodLossRate*dt;
        if (mBloodLevel<0.0f)
        {
            mIsDead=true;
            return true;
        }

        if (mBloodLossRate>0.0f)
        {
            owner->mind()->adjustHappiness(-0.0833333358168602f*dt);
            mSpawnBloodTimer-=dt*mBloodLossRate;
            if (mSpawnBloodTimer<0.0f)
            {
                mSpawnBloodTimer=RANDOMIZER->uniform(0.0f,0.5f);
            }
        }

        if (mIsSleeping)
        {
            mRestLevel+=1.0f/(6.0f*raceDef->RestTime/mRestRate)*dt;
        }
        else if (raceDef->ExhaustionTime>0.0f)
        {
            mRestLevel-=1.0f/(6.0f*raceDef->ExhaustionTime)*dt/fitness;
        }

        if (raceDef->HungerRate>0.0f)
        {
            mHungerLevel-=1.0f/(6.0f*raceDef->HungerRate)*dt/fitness;
        }

        if (mHungerLevel<0.0f)
        {
            mIsDead=true;
            return true;
        }

        if (isStarving())
        {
            owner->mind()->adjustHappiness(-0.0833333358168602f*dt);
        }

        if (raceDef->ThirstRate>0.0f)
        {
            mThirstLevel-=1.0f/(6.0f*raceDef->ThirstRate)*dt/fitness;
        }

        if (mThirstLevel<0.0f)
        {
            mIsDead=true;
            return true;
        }

        if (isDyingOfThirst())
        {
            owner->mind()->adjustHappiness(-0.0833333358168602f * dt);
        }

        if ((mBodyFunctionTotal.at((int)BodyFunction::Breathe)!=0) && (owner->cell()->willSuffocate()))
        {
            mLungCapacity-=10.0f*dt;
            if (mLungCapacity<0.0f)
            {
                mIsDead=true;
                return true;
            }
        }
        else
        {
            mLungCapacity=100.0f;
        }

        if (mGestationTime>0.0f)
        {
            mGestationTime-=dt;
            if (mGestationTime<0.0f)
            {
                giveBirth();
            }
        }

        float velocity=owner->attributeLevel(CharacterAttributeType::Nimbleness);
        for (const auto& effect : mStatusEffects)
        {
            if (effect.Ailment==HealthStatusAilment::Unconcious)
            {
                velocity=0.0f;
                break;
            }

            velocity*=0.9f;
        }

        float dodgeModifier=1.0f;
        const auto squad_pos=owner->squadPosition();
        if ((squad_pos!=nullptr) && (squad_pos->perk()==SquadPositionPerk::WayOfTheGnome)
            && (mEquippedItems.empty()) && (mHeldItems.empty()))
        {
            const float dodge=owner->skillLevel(CHARACTER_SETTINGS->DodgeSkill);
            dodgeModifier=1.0f+dodge*(1.0f+dodge/200.0f)/100.0f;
        }

        if (mDodgeTimer>0.0f)
            mDodgeTimer-=dt*velocity*dodgeModifier;

        if (mPickupWeaponTimer>0.0f)
            mPickupWeaponTimer-=dt*velocity;

        float naturalAttackModifier=1.0f;
        if ((squad_pos!=nullptr) && (squad_pos->perk()==SquadPositionPerk::WayOfTheGnome) && (mEquippedItems.empty()))
        {
            const float naturalAttack=owner->skillLevel(NATURALATTACKSKILLID);
            naturalAttackModifier=1.0f+naturalAttack/100.0f;
        }

        for (const auto& weapon : mNaturalWeapons)
            weapon->update(dt*velocity*naturalAttackModifier);

        return true;
    }

    void Body::giveBirth()
    {

    }

    std::vector<std::shared_ptr<Item>> Body::heldItems() const
    {
        return mHeldItems;
    }

    size_t Body::heldItemsCount() const
    {
        return mHeldItems.size();
    }

    bool Body::hasItem(std::shared_ptr<Item> item) const
    {
        auto iter=std::find_if(mHeldItems.begin(),mHeldItems.end(),[&item](std::shared_ptr<Item> const& value)
        {
            return item->ID()==value->ID();
        });

        return (iter!=mHeldItems.end());
    }

    std::vector<std::shared_ptr<Item>> Body::equippedItems() const
    {
        return mEquippedItems;
    }

    size_t Body::equippedItemsCount() const
    {
        return mEquippedItems.size();
    }

    const bool& Body::isLookingForFood() const
    {
        return mLookingForFood;
    }

    void Body::setLookingForFood()
    {
        mLookingForFood=true;
    }

    bool Body::canCarry(std::vector<std::shared_ptr<BodySection>> sectionList) const
    {
        for (const auto& section : sectionList)
        {
            if (section->canCarry())
                return true;
        }

        return false;
    }

    bool Body::canCarry(EquipmentType equipType) const
    {
        auto iter=std::find_if(mEquipmentSlots.begin(),mEquipmentSlots.end(),[&equipType](BodySectionInfo const& value)
        {
            return equipType==value.type;
        });

        if (iter==mEquipmentSlots.end())
            return false;

        return canCarry(iter->sections);
    }

    bool Body::canCarry() const
    {
        if (!canCarry(EquipmentType::LeftHand))
            return canCarry(EquipmentType::RightHand);

        return true;
    }

    bool Body::canCarry(std::shared_ptr<Item> item) const
    {
        if (!GAME_DEFINITIONS->itemDefinition(item->itemID())->TwoHanded)
            return canCarry();

        if (canCarry(EquipmentType::LeftHand))
            return canCarry(EquipmentType::RightHand);

        return false;
    }

    std::vector<std::shared_ptr<Item>> Body::getDropedItem(EquipmentType equipType)
    {
        std::vector<std::shared_ptr<Item>> result;
        if ((equipType!=EquipmentType::LeftHand) && (equipType!=EquipmentType::RightHand))
            return result;

        auto iter=std::find_if(mEquipmentSlots.begin(),mEquipmentSlots.end(),[&equipType](BodySectionInfo const& value)
        {
            return value.type==equipType;
        });

        if (iter==mEquipmentSlots.end())
            return result;

        for (const auto& section : iter->sections)
        {
            auto heldItem=section->heldItem();
            if (heldItem!= nullptr)
            {
                if (dropItem(heldItem))
                    result.push_back(heldItem);
            }
        }

        return result;
    }

    bool Body::sheathItem()
    {
        auto iter=std::find_if(mAdditionalEquipmentSlots.begin(),mAdditionalEquipmentSlots.end(),[](EquipItemInfo const& value)
        {
            return value.type==EquipmentType::Belt;
        });

        if (iter!=mAdditionalEquipmentSlots.end())
            return false;

        std::vector<std::shared_ptr<Item>> objects;
        if (!canCarry(EquipmentType::RightHand))
        {
            auto items=getDropedItem(EquipmentType::RightHand);
            for (const auto& item : items)
                objects.push_back(item);
        }

        if ((objects.empty()) && (!canCarry(EquipmentType::LeftHand)))
        {
            auto items=getDropedItem(EquipmentType::LeftHand);
            for (const auto& item : items)
                objects.push_back(item);
        }

        if (objects.empty())
            return false;

        EquipItemInfo new_add_slot;
        new_add_slot.type=EquipmentType::Belt;
        new_add_slot.item=objects.at(0);
        mAdditionalEquipmentSlots.push_back(new_add_slot);

        auto object_id=objects.at(0)->ID();
        auto iter_equip=std::find_if(mEquippedItems.begin(),mEquippedItems.end(),[&object_id](std::shared_ptr<Item> const& value)
        {
            return value->ID()==object_id;
        });

        if (iter_equip==mEquippedItems.end())
        {
            mCombatValue += objects.at(0)->combatValue();
            mMovementPenalty += objects.at(0)->movementPenalty();
            mJobPenalty += objects.at(0)->jobPenalty();
            mEquippedItems.push_back(objects.at(0));
        }

        return true;
    }

    void Body::eatFoot(std::shared_ptr<Item> item)
    {
        auto additionalDiet=owner->raceDefinition()->additionalDietValue(item->itemID());

        mHungerLevel+=(item->effectAmount(ItemEffectType::Food)+additionalDiet)*owner->raceDefinition()->FoodRatio;
        dropItem(item);

        if (mHungerLevel<=100.0f)
            return;

        mLookingForFood=false;
    }

    std::vector<properties::TileDef> Body::getTiles() const
    {
        std::vector<properties::TileDef> result;
        mMainBodySection->getTiles(result);
        return result;
    }

    GenderType Body::gender() const
    {
        return owner->gender();
    }

    bool Body::isSuffocating() const
    {
        return (mLungCapacity<100.0f);
    }

    const float& Body::combatValue() const
    {
        return mCombatValue;
    }

    bool Body::canPickupWeapon() const
    {
        return (mPickupWeaponTimer<=0.0f);
    }

    std::vector<std::shared_ptr<Weapon>> Body::naturalWeapons() const
    {
        return mNaturalWeapons;
    }

    std::vector<std::shared_ptr<Item>> Body::additionalEquippedItemsInSlot(EquipmentType equipType) const
    {
        std::vector<std::shared_ptr<Item>> result;

        for (const auto& item : mAdditionalEquipmentSlots)
        {
            if ((item.type==equipType) && (item.item!= nullptr))
                result.push_back(item.item);
        }

        return result;
    }

    std::vector<std::shared_ptr<Item>> Body::equippedItemsInSlot(EquipmentType equipType) const
    {
        if (!isEqupmentSlotsContain(equipType))
            return additionalEquippedItemsInSlot(equipType);

        std::vector<std::shared_ptr<Item>> result;
        for (const auto& item : mEquipmentSlots)
        {
            if (item.type==equipType)
            {
                for (const auto& section : item.sections)
                {
                    const auto equippedItem=section->equippedItem();
                    if (equippedItem!= nullptr)
                        result.push_back(equippedItem);
                }
            }
        }

        return result;
    }

    int Body::equippedItemsInSlotCount(EquipmentType equipType) const
    {
        const auto items=equippedItemsInSlot(equipType);
        return (int)items.size();
    }

    bool Body::hasAmmo(std::shared_ptr<const Weapon> weapon) const
    {
        const auto ammoItemID=weapon->weaponDef()->AmmoItemID;
        if (ammoItemID.empty())
            return true;

        const auto equippedItems=equippedItemsInSlot(EquipmentType::Back);
        for (const auto& item : equippedItems)
        {
            const auto containers=ITEM_SETTINGS->containersByAmmoItemID(ammoItemID);
            const auto itemID=item->itemID();

            auto iter=std::find_if(containers.begin(),containers.end(),[&itemID](std::string const& value)
            {
                return value==itemID;
            });

            if (iter!=containers.end())
            {
                const auto storageContainer=std::dynamic_pointer_cast<StorageContainer>(item);
                if ((storageContainer!=nullptr) && (storageContainer->containedResourcesCount()>0))
                    return true;
            }
        }

        return false;
    }

    bool Body::hasWeaponInRange(float distance) const
    {
        for (const auto& weapon : mHeldWeapons)
        {
            if (weapon->withinRange(distance,owner))
                return true;
        }

        if (!mHeldWeapons.empty())
            return false;

        for (const auto& weapon : mNaturalWeapons)
        {
            if (weapon->withinRange(distance,owner))
                return true;
        }

        return false;
    }

    float Body::maxRange() const
    {
        float result=1.0f;

        for (const auto& weapon : mHeldWeapons)
        {
            float range=weapon->maxRange(owner);
            if (range>result)
                result=range;
        }

        if (!mHeldWeapons.empty())
            return result;

        for (const auto& weapon : mNaturalWeapons)
        {
            float range=weapon->maxRange(owner);
            if (range>result)
                result=range;
        }

        return result;
    }

    bool Body::isBleeding() const
    {
        return (mBloodLossRate>0.0f);
    }

    std::vector<std::shared_ptr<Weapon>> Body::weapons(float distance) const
    {
        return weapons(true,distance);
    }

    std::vector<std::shared_ptr<Weapon>> Body::weapons(bool rangeCheck, float distance) const
    {
        std::vector<std::shared_ptr<Weapon>> result;

        for (const auto& weapon : mHeldWeapons)
        {
            if ((weapon->canAttack()) && ((!rangeCheck) || (weapon->withinRange(distance,owner))))
                result.push_back(weapon);
        }

        if (!result.empty())
            return result;

        for (const auto& weapon : mNaturalWeapons)
        {
            if ((weapon->canAttack()) && ((!rangeCheck) || (weapon->withinRange(distance,owner))))
                result.push_back(weapon);
        }

        return result;
    }

    float Body::adjustHitWeightByPerk(std::shared_ptr<const Character> attacker, SquadPositionPerk perk,
            std::shared_ptr<const BodySection> section) const
    {
        switch (perk)
        {
            case SquadPositionPerk::Cripple:
                if ((section->hasFunction(BodyFunction::Stand)) || (section->directlyConnectsToFunction(BodyFunction::Stand)))
                    return 1.5f;
                break;
            case SquadPositionPerk::Disarm:
                if ((section->hasFunction(BodyFunction::Grip)) || (section->directlyConnectsToFunction(BodyFunction::Grip)))
                    return 1.5f;
                break;
            case SquadPositionPerk::Blind:
                if ((section->hasFunction(BodyFunction::Sight)) || (section->directlyConnectsToFunction(BodyFunction::Sight)))
                    return 1.5f;
                break;
            case SquadPositionPerk::Highlander:
                if ((attacker->heldItemsCount()==1) && (attacker->equippedItemsCount()==0))
                {
                    const auto itemID=attacker->heldItems().at(0)->itemID();
                    if (GAME_DEFINITIONS->itemDefinition(itemID)->TwoHanded)
                        return 3.0f;
                }
                break;
            default:
                return 1.0f;
        }

        return 1.0f;
    }

    std::shared_ptr<BodySection> Body::randomSection(std::shared_ptr<const Character> attacker) const
    {
        float summAttackValue=0.0f;
        std::shared_ptr<SquadPosition> squad_pos=nullptr;
        if (attacker!=nullptr)
            squad_pos=attacker->squadPosition();

        for (const auto& bodySection : mBodySections)
        {
            if ((!bodySection->hasStatus(BodySectionStatus::Missing)) && (!bodySection->hasStatus(BodySectionStatus::Destroyed)))
            {
                float toHitWeight=bodySection->toHitWeight();
                if (squad_pos!= nullptr)
                    toHitWeight*=adjustHitWeightByPerk(attacker,squad_pos->perk(),bodySection);

                summAttackValue+=toHitWeight;
            }
        }

        float rand=RANDOMIZER->uniform();
        const float randAttackValue=rand*summAttackValue;
        float currentAttackValue=0.0f;
        for (const auto& bodySection : mBodySections)
        {
            if ((!bodySection->hasStatus(BodySectionStatus::Missing)) && (!bodySection->hasStatus(BodySectionStatus::Destroyed)))
            {
                float toHitWeight=bodySection->toHitWeight();
                if (squad_pos!= nullptr)
                    toHitWeight*=adjustHitWeightByPerk(attacker,squad_pos->perk(),bodySection);

                currentAttackValue+=toHitWeight;

                if (randAttackValue<currentAttackValue)
                    return bodySection;
            }
        }

        return nullptr;
    }

    std::shared_ptr<Item> Body::consumeAmmo(const std::string& ammoType, bool deleteAmmo) const
    {
        if (ITEM_SETTINGS->itemIDToAmmoID(ammoType)==AMMOID_NONE)
            return nullptr;

        const auto equiped=equippedItemsInSlot(EquipmentType::Back);
        for (const auto& item : equiped)
        {
            const auto storageContainer=std::dynamic_pointer_cast<StorageContainer>(item);
            if (storageContainer!=nullptr)
            {
                const auto containedResource=storageContainer->containedResources().back();
                if (containedResource->itemID()==ammoType)
                {
                    if (deleteAmmo)
                    {
                        storageContainer->removeItem(containedResource);
                        containedResource->toDestroy();
                    }

                    return containedResource;
                }
            }
        }

        return nullptr;
    }

    bool Body::isSleeping() const
    {
        return mIsSleeping;
    }

    std::vector<std::shared_ptr<Weapon>> Body::defenseWeapons() const
    {
        std::vector<std::shared_ptr<Weapon>> result;
        for (const auto& weapon : mHeldWeapons)
        {
            if (weapon->canDefend())
                result.push_back(weapon);
        }

        if (!result.empty())
            return result;

        for (const auto& weapon : mNaturalWeapons)
        {
            if (weapon->canDefend())
                result.push_back(weapon);
        }

        return result;
    }

    bool Body::canDodge() const
    {
        return (mDodgeTimer<=0.0f);
    }

    void Body::dodged()
    {
        mDodgeTimer+=owner->raceDefinition()->DodgeTime;
    }

    void Body::wakeUp()
    {
        owner->setNeedGoal(nullptr);
        mIsSleeping=false;
    }

    std::shared_ptr<Character> Body::character() const
    {
        return owner;
    }

    void Body::addNaturalWeapon(std::shared_ptr<Weapon> weapon)
    {
        mNaturalWeapons.push_back(weapon);
    }

    void Body::removeNaturalWeapon(std::shared_ptr<Weapon> weapon)
    {
        auto iter=std::find_if(mNaturalWeapons.begin(),mNaturalWeapons.end(),[&weapon](std::shared_ptr<Weapon> const& elem)
        {
            return elem.get()==weapon.get();
        });

        if (iter!=mNaturalWeapons.end())
            mNaturalWeapons.erase(iter);
    }

    bool Body::remove(std::shared_ptr<Item> item)
    {
        const auto iter=std::find_if(mEquippedItems.begin(),mEquippedItems.end(),[&item](std::shared_ptr<Item> const& elem)
        {
            return item->ID()==elem->ID();
        });

        if (iter==mEquippedItems.end())
            return false;

        mEquippedItems.erase(iter);
        mCombatValue-=item->combatValue();
        mMovementPenalty-=item->movementPenalty();
        mJobPenalty-=item->jobPenalty();
        return true;
    }

    bool Body::unequipAdditional(std::shared_ptr<Item> item)
    {
        const auto iter=std::find_if(mEquippedItems.begin(),mEquippedItems.end(),[&item](std::shared_ptr<Item> const& elem)
        {
            return item->ID()==elem->ID();
        });

        if (iter==mEquippedItems.end())
            return false;

        const auto equpSlot=GAME_DEFINITIONS->itemDefinition(item->itemID())->EquipSlot;

        const auto iterAdditional=std::find_if(mAdditionalEquipmentSlots.begin(),mAdditionalEquipmentSlots.end(),
                [&equpSlot](EquipItemInfo const& elem)
        {
            return elem.type==equpSlot;
        });

        if (iterAdditional==mAdditionalEquipmentSlots.end())
            return false;

        if (iterAdditional->item->ID()!=item->ID())
            return false;

        mAdditionalEquipmentSlots.erase(iterAdditional);

        remove(item);
        return true;
    }

    std::vector<std::shared_ptr<BodySection>> Body::getSectionsByEqupSlot(const EquipmentType& equipType) const
    {
        const auto iter=std::find_if(mEquipmentSlots.begin(),mEquipmentSlots.end(),[&equipType](BodySectionInfo const& elem)
        {
            return elem.type==equipType;
        });

        if (iter!=mEquipmentSlots.end())
            return iter->sections;
        else
            return std::vector<std::shared_ptr<BodySection>>();
    }

    bool Body::unequip(std::shared_ptr<Item> item)
    {
        auto iter=std::find_if(mEquippedItems.begin(),mEquippedItems.end(),[&item](std::shared_ptr<Item> const& elem)
        {
            return item->ID()==elem->ID();
        });

        if (iter==mEquippedItems.end())
            return false;

        const auto equipSlot=GAME_DEFINITIONS->itemDefinition(item->itemID())->EquipSlot;
        if (!isEqupmentSlotsContain(equipSlot))
        {
            return unequipAdditional(item);
        }

        const auto sectionList=getSectionsByEqupSlot(equipSlot);
        for (const auto& section : sectionList)
        {
            if (section->equippedItem()->ID()==item->ID())
            {
                remove(item);
                section->unequip(item);
                return true;
            }
        }

        return false;
    }

    void Body::removeArtificialLimb(std::shared_ptr<Item> item)
    {
        if ((item->parent()!=nullptr) && (item->parent()->ID()!=owner->ID()))
        {
            removeArtificialLimb(std::dynamic_pointer_cast<Item>(item->parent()));
        }
        else
        {
            for (const auto& section : mBodySections)
            {
                if (section->bodyPart()->prostheticPart()->ID()==item->ID())
                {
                    section->removeArtificialPart();
                    item->moveTo(owner->position(),false);
                    item->toDeconstruct();
                    break;
                }
            }
        }
    }

    float Body::combinedBloodLossRate() const
    {
        float result=0.0f;

        for (const auto& section : mBodySections)
            result+=section->bloodLossRate();

        return result;
    }

    void Body::combineStatusEffects(const std::vector<BodySectionStatus>& oldStatus,std::shared_ptr<BodySection> section,
                              std::vector<std::string>& statusEffects) const
    {
        const auto newStatus=section->status();

        if (oldStatus.size()==newStatus.size())
        {
            bool check=true;
            for (const auto& status : newStatus)
            {
                const auto iter=std::find_if(oldStatus.begin(),oldStatus.end(),[&status](BodySectionStatus const& elem)
                {
                    return elem==status;
                });

                if (iter==oldStatus.end())
                {
                    check=false;
                    break;
                }
            }

            if (check)
                return;
        }

        std::string log;

        if (!owner->hasUniqueName())
            log+="The ";

        log+=owner->name();
        statusEffects.push_back(log);

        for (size_t i=0,isize=(size_t)BodySectionStatus::Undef;i<isize;i++)
        {
            const auto status=BodySectionStatus(i);

            const auto iterOld=std::find_if(oldStatus.begin(),oldStatus.end(),[&status](BodySectionStatus const& elem)
            {
                return elem==status;
            });

            const auto iterNew=std::find_if(newStatus.begin(),newStatus.end(),[&status](BodySectionStatus const& elem)
            {
                return elem==status;
            });

            if ((iterOld==oldStatus.end()) && (iterNew!=newStatus.end()))
            {
                if (status==BodySectionStatus::Destroyed)
                {
                    if (statusEffects.size()==1)
                    {
                        statusEffects[0]+="'s";
                        statusEffects.push_back(section->name()+" has been mangled");
                        continue;
                    }

                    statusEffects.push_back(owner->history()->possessivePronoun()+" "+section->name()+" has been mangled");
                }
                else if (status==BodySectionStatus::Missing)
                {
                    if (section->isConnection())
                    {
                        const auto connections=section->connectsTo();
                        for (const auto& connection : connections)
                        {
                            statusEffects.push_back("lost "+owner->history()->possessivePronoun()+" "+connection->name());
                            owner->lostLimb(connection);
                        }
                    }
                    else
                    {
                        statusEffects.push_back("lost "+owner->history()->possessivePronoun()+" "+section->name());
                        owner->lostLimb(section);
                    }
                }
                else if (status==BodySectionStatus::Bleeding)
                {
                    if (statusEffects.size()==1)
                    {
                        statusEffects[0]+="'s";
                        statusEffects.emplace_back(section->name()+" is bleeding");
                    }

                    statusEffects.emplace_back(owner->history()->possessivePronoun()+" "+section->name()+" is bleeding");
                }
                else if (status==BodySectionStatus::StruckArtery)
                {
                    statusEffects.emplace_back("an artery has been struck");
                }
                else if (status==BodySectionStatus::InternalBleeding)
                {
                    statusEffects.emplace_back("has internal bleeding");
                }
            }
        }
    }

    void Body::combineDamageFunctions(std::vector<std::string>& statusEffects)
    {
        if ((statusEffects.empty()) && (!mDamagedFunction.empty()))
        {
            std::string log;
            if (!owner->hasUniqueName())
                log+="The ";

            log+=owner->name();
            statusEffects.push_back(log);
        }

        for (size_t i=0,isize=mDamagedFunction.size();i<isize;i++)
        {
            if (mDamagedFunction.at(i))
            {
                const auto func=BodyFunction(i);
                const auto rand1=RANDOMIZER->uniform(0.0f,1.0f);
                const auto rand2=RANDOMIZER->uniform(0.0f,1.0f);
                if (func==BodyFunction::Thought)
                {
                    statusEffects.emplace_back("is dazed");
                    addStatusEffect(HealthStatusAilment::Dazed,rand1*2.0f+2.0f,rand2*0.5f+1.0f,false);
                }
                else if (func==BodyFunction::Circulation)
                {
                    if (std::dynamic_pointer_cast<Automaton>(owner)!=nullptr)
                        statusEffects.emplace_back("is losing power");
                    else
                        statusEffects.emplace_back("is feeling faint");

                    addStatusEffect(HealthStatusAilment::Faint,rand1*2.0f+2.0f,rand2*0.5f+1.0f,false);
                }
                else if (func==BodyFunction::Breathe)
                {
                    statusEffects.emplace_back("is having trouble breathing");
                    addStatusEffect(HealthStatusAilment::Winded,rand1*2.0f+2.0f,rand2*0.5f+1.0f,false);
                }
                else if (func==BodyFunction::Throat)
                {
                    statusEffects.emplace_back("is suffocating");
                    addStatusEffect(HealthStatusAilment::Winded,rand1*2.0f+2.0f,rand2*0.5f+1.0f,false);
                }
                else if (func==BodyFunction::Stand)
                {
                    if (abilityLevel(BodyFunction::Stand)>0.5f)
                    {
                        if (!containStatusEffect(HealthStatusAilment::FallenOver))
                        {
                            statusEffects.emplace_back("falls to the ground");
                        }
                    }

                    addStatusEffect(HealthStatusAilment::FallenOver,rand1*2.0f+2.0f,rand2*0.5f+1.0f,false);
                }
                else if (func==BodyFunction::Wing)
                {
                    if (abilityLevel(BodyFunction::Wing)>0.5f)
                    {
                        if (!containStatusEffect(HealthStatusAilment::FallenOver))
                        {
                            statusEffects.emplace_back("falls to the ground");
                        }
                    }

                    addStatusEffect(HealthStatusAilment::FallenOver,rand1*2.0f+2.0f,rand2*0.5f+1.0f,false);
                }
            }
        }
    }

    void Body::combineLostFunctions(std::vector<std::string>& statusEffects)
    {
        if ((statusEffects.empty()) && (!mDamagedFunction.empty()))
        {
            std::string log;
            if (!owner->hasUniqueName())
                log+="The ";

            log+=owner->name();
            statusEffects.push_back(log);
        }

        for (size_t i=0,isize=mLostFunction.size();i<isize;i++)
        {
            if (mLostFunction.at(i))
            {
                const auto func = BodyFunction(i);
                const auto rand1 = RANDOMIZER->uniform(0.0f, 1.0f);
                const auto rand2 = RANDOMIZER->uniform(0.0f, 1.0f);

                if (func==BodyFunction::Thought)
                {
                    if (mBodyFunctionCurrent.at(i)>0)
                    {
                        statusEffects.emplace_back("is wracked with pain");
                        addStatusEffect(HealthStatusAilment::Dazed,rand1*2.0f+2.0f,rand2*0.5f+1.0f,false);
                    }
                }
                else if (func==BodyFunction::Circulation)
                {
                    if (mBodyFunctionCurrent.at(i)>0)
                    {
                        if (std::dynamic_pointer_cast<Automaton>(owner) != nullptr)
                            statusEffects.emplace_back("is losing power");
                        else
                            statusEffects.emplace_back("is feeling faint");

                        addStatusEffect(HealthStatusAilment::Faint, rand1 * 2.0f + 2.0f, rand2 * 0.5f + 1.0f, false);
                    }
                }
                else if (func==BodyFunction::Sight)
                {
                    if (mBodyFunctionCurrent.at(i)==0)
                    {
                        statusEffects.emplace_back("is blind");
                        addStatusEffect(HealthStatusAilment::Blind, 1.0f, 0.0f, true);
                    }
                    else
                    {
                        statusEffects.push_back(owner->history()->pronoun()+" is temporarily blinded");
                        addStatusEffect(HealthStatusAilment::Blind, rand1 * 2.0f + 2.0f, rand2 * 0.5f + 1.0f, false);
                    }
                }
                else if (func==BodyFunction::Breathe)
                {
                    if (mBodyFunctionCurrent.at(i)==0)
                    {
                        statusEffects.emplace_back("is suffocating");
                    }
                    else
                    {
                        statusEffects.emplace_back("is having trouble breathing");
                        addStatusEffect(HealthStatusAilment::Winded, rand1 * 2.0f + 2.0f, rand2 * 0.5f + 1.0f, false);
                    }
                }
                else if (func==BodyFunction::Throat)
                {
                    statusEffects.emplace_back("is suffocating");
                }
                else if (func==BodyFunction::Stand)
                {
                    if (abilityLevel(BodyFunction::Stand)<=0.5f)
                    {
                        std::string log;
                        if (!statusEffects.empty())
                            log+=owner->history()->pronoun();

                        log+=" can no longer walk";
                        statusEffects.push_back(log);
                        addStatusEffect(HealthStatusAilment::FallenOver,1.0f,0.0f,true);
                    }
                }
                else if (func==BodyFunction::Wing)
                {
                    if (abilityLevel(BodyFunction::Wing)<=0.5f)
                    {
                        std::string log;
                        if (!statusEffects.empty())
                            log+=owner->history()->pronoun();

                        log+=" can no longer fly";
                        statusEffects.push_back(log);
                        addStatusEffect(HealthStatusAilment::FallenOver,1.0f,0.0f,true);
                    }
                }
            }
        }
    }

    void Body::combineAilments(const std::vector<HealthStatusAilment>& oldStatus, std::vector<std::string>& statusEffects) const
    {
        if (oldStatus.size()==mStatusEffects.size())
        {
            bool check=true;
            for (const auto& status : oldStatus)
            {
                if (!containStatusEffect(status))
                {
                    check=false;
                    break;
                }
            }

            if (check)
                return;
        }

        if (statusEffects.empty())
        {
            std::string log;
            if (!owner->hasUniqueName())
                log+="The ";

            log+=owner->name();
            statusEffects.push_back(log);
        }

        for (const auto& effect : mStatusEffects)
        {
            const auto status=effect.Ailment;

            const auto iter=std::find_if(oldStatus.begin(),oldStatus.end(),[&status](HealthStatusAilment const& elem)
            {
                return elem==status;
            });

            if (iter==oldStatus.end())
            {
                if (status==HealthStatusAilment::ZombieVirus)
                {
                    statusEffects.emplace_back("is infected with the zombie virus");
                }
            }
        }
    }

    void Body::damage(std::shared_ptr<Attack> attack, std::shared_ptr<BodySection> sectionHit, std::vector<std::string>& hitEffects,
                std::vector<std::string>& statusEffects)
    {
        auto iter=std::find_if(mBodySections.begin(),mBodySections.end(),[&sectionHit](std::shared_ptr<BodySection> const& elem)
        {
            return elem->ID()==sectionHit->ID();
        });

        if (iter==mBodySections.end())
            return;

        wakeUp();

        std::fill(mDamagedFunction.begin(),mDamagedFunction.end(),false);
        std::fill(mLostFunction.begin(),mLostFunction.end(),false);
        mItemsToDrop.clear();

        const auto sectionStatus=sectionHit->status();

        std::vector<HealthStatusAilment> status;
        for (const auto& effect : mStatusEffects)
            status.push_back(effect.Ailment);

        sectionHit->damage(attack,hitEffects);

        mBloodLossRate=combinedBloodLossRate();
        combineStatusEffects(sectionStatus,sectionHit,statusEffects);
        combineDamageFunctions(statusEffects);
        combineLostFunctions(statusEffects);
        combineAilments(status,statusEffects);
    }

    std::vector<std::shared_ptr<Item>> Body::itemsToDrop() const
    {
        return mItemsToDrop;
    }

    size_t Body::itemsToDropCount() const
    {
        return mItemsToDrop.size();
    }

    void Body::repairEquipment()
    {
        for (const auto& section : mBodySections)
            section->repairEquipment();
    }

    std::vector<std::shared_ptr<BodySection>> Body::bodySections() const
    {
        return mBodySections;
    }
}