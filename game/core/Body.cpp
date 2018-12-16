//
// Created by tatiana on 26.03.2018.
//

#include "Body.h"
#include "defines.h"
#include "Character.h"
#include "HealthStatusEffect.h"
#include "SquadPosition.h"
#include "BodySection.h"
#include "BodyPart.h"
#include "Mind.h"
#include "GameManager.h"
#include "Item.h"
#include "game/properties/RaceDefinition.h"
#include "game/properties/GameDefinition.h"
#include "game/properties/BodyDef.h"
#include "game/utils/math_utils.h"

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
        std::shared_ptr<Body> ptr=std::shared_ptr<Body>(new Body());
        ptr->init(parent);
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

        mBodyFunctionTotal.resize((int)BodyFunction::Count);
        std::fill(std::begin(mBodyFunctionTotal),std::end(mBodyFunctionTotal),0);
        mBodyFunctionCurrent.resize((int)BodyFunction::Count);
        std::fill(std::begin(mBodyFunctionCurrent),std::end(mBodyFunctionCurrent),0);

        auto bodyDef=GAME_DEFINITIONS->bodyDefinition(raceDef->BodyID);
        bodySectionsKey=0;
        mMainBodySection=BodySection::create(bodySectionsKey,shared_from_this(),bodyDef->MainBody,BodySymmetricalType::None);
    }

    float Body::exhaustionModifier() const
    {
        return utils::clamp(mRestLevel/owner->raceDefinition()->TiredLevel,0.5f,1.0f);
    }

    bool Body::containStatusEffect(HealthStatusAilment key) const
    {
        auto iter=std::find_if(std::begin(mStatusEffects),std::end(mStatusEffects),[&key](HealthStatusEffect const& value)
        {
            return (value.Ailment==key);
        });

        return (iter!=std::end(mStatusEffects));
    }

    float Body::movementPenalty() const
    {
        return (1.0f-mMovementPenalty*0.00999999977648258);
    }

    float Body::moveSpeed() const
    {
        if (mIsSleeping)
            return 0.0f;

        float value=owner->raceDefinition()->MoveSpeed*this->exhaustionModifier()*owner->attributeLevel(CharacterAttributeType::Nimbleness);

        if (this->containStatusEffect(HealthStatusAilment::FallenOver))
            value=value*0.1f;

        value=value*this->movementPenalty();

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
        auto iter=std::find_if(std::begin(mEquipmentSlots),std::end(mEquipmentSlots),[&equipType](BodySectionInfo const& value)
        {
            return equipType==value.type;
        });

        if (iter==std::end(mEquipmentSlots))
        {
            BodySectionInfo newInfo;
            newInfo.type=equipType;
            newInfo.sections.push_back(section);
            mEquipmentSlots.push_back(newInfo);
        }
        else
        {
            auto iter_section=std::find_if(std::begin(iter->sections),std::end(iter->sections),[&section](std::shared_ptr<BodySection> const& value)
            {
                return section->ID==value->ID;
            });

            if (iter_section==std::end(iter->sections))
                return;
            else
            {
                int index=std::distance(mEquipmentSlots.begin(),iter);
                mEquipmentSlots[index].sections.push_back(section);
            }
        }
    }

    void Body::addFunction(std::shared_ptr<BodyPart> bodyPart)
    {
        auto function=bodyPart->function();
        if (function==BodyFunction::None)
            return;

        mBodyFunctionTotal[(int)function]+=1;
        mBodyFunctionCurrent[(int)function]+=1;
    }

    bool Body::pickupItem(std::shared_ptr<Item> item, std::vector<std::shared_ptr<BodySection>> sections)
    {
        for (auto section : sections)
        {
            if (section->canCarry())
            {
                section->carryItem(item,true);

                auto iter=std::find_if(std::begin(mHeldItems),std::end(mHeldItems),[&item](std::shared_ptr<Item> const& value)
                {
                    return item->ID()==value->ID();
                });

                if (iter==std::end(mHeldItems))
                    mHeldItems.push_back(item);

                return true;
            }
        }

        return false;
    }

    bool Body::isEqupmentSlotsContain(EquipmentType equipType) const
    {
        auto iter=std::find_if(std::begin(mEquipmentSlots),std::end(mEquipmentSlots),[&equipType](BodySectionInfo const& value)
        {
            return equipType==value.type;
        });

        return (iter!=std::end(mEquipmentSlots));
    }

    bool Body::pickupItem(std::shared_ptr<Item> item, EquipmentType equipType)
    {
        auto iter=std::find_if(std::begin(mEquipmentSlots),std::end(mEquipmentSlots),[&equipType](BodySectionInfo const& value)
        {
            return equipType==value.type;
        });

        if (iter==std::end(mEquipmentSlots))
            return false;

        this->pickupItem(item,iter->sections);
    }

    bool Body::pickupItem(std::shared_ptr<Item> item)
    {
        if (this->pickupItem(item,EquipmentType::LeftHand))
            return true;

        return this->pickupItem(item,EquipmentType::RightHand);
    }

    float Body::abilityLevel(BodyFunction bodyFunction)
    {
        if (mBodyFunctionTotal.at((int)bodyFunction)!=0)
            return float(mBodyFunctionCurrent.at((int)bodyFunction))/float(mBodyFunctionTotal.at((int)bodyFunction));

        return (bodyFunction==BodyFunction::Wing) ? 0.0f : 1.0f;
    }

    void Body::addEquipmentSlot(std::shared_ptr<BodySection> section)
    {
        if (section->equipType()==EquipmentType::None)
            return;

        this->addEquipmentSlot(section,section->equipType());
    }

    void Body::addSection(std::shared_ptr<BodySection> section)
    {
        auto iter=std::find_if(std::begin(mBodySections),std::end(mBodySections),[&section](std::shared_ptr<BodySection> const& value)
        {
            return section->ID==value->ID;
        });

        if (iter==std::end(mBodySections))
        {
            mBodySections.push_back(section);
            this->addEquipmentSlot(section);
        }
    }

    bool Body::dropItem(std::shared_ptr<Item> item, std::vector<std::shared_ptr<BodySection>> sections)
    {
        if (sections.size()==0)
            return false;

        for (auto section : sections)
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
        auto iter=std::find_if(std::begin(mHeldItems),std::end(mHeldItems),[&item](std::shared_ptr<Item> const& value)
        {
            return item->ID()==value->ID();
        });

        if (iter==std::end(mHeldItems))
            return false;

        auto iter_equip_left=std::find_if(std::begin(mEquipmentSlots),std::end(mEquipmentSlots),[](BodySectionInfo const& value)
        {
            return value.type==EquipmentType::LeftHand;
        });

        if (iter_equip_left!=std::end(mEquipmentSlots))
        {
            if (this->dropItem(item,iter_equip_left->sections))
            {
                mHeldItems.erase(iter);
                return true;
            }
        }

        auto iter_equip_right=std::find_if(std::begin(mEquipmentSlots),std::end(mEquipmentSlots),[](BodySectionInfo const& value)
        {
            return value.type==EquipmentType::RightHand;
        });

        if (iter_equip_right!=std::end(mEquipmentSlots))
        {
            if (this->dropItem(item,iter_equip_right->sections))
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
        this->dropItem(item);

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
            owner->mind->adjustHappiness(-0.0833333358168602f*dt);
            mSpawnBloodTimer-=dt*mBloodLossRate;
            if (mSpawnBloodTimer<0.0f)
            {
                mSpawnBloodTimer=0.5f;
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

        if (this->isStarving())
        {
            owner->mind->adjustHappiness(-0.0833333358168602f*dt);
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

        if (this->isDyingOfThirst())
        {
            owner->mind->adjustHappiness(-0.0833333358168602f * dt);
        }

        if (mGestationTime>0.0f)
        {
            mGestationTime-=dt;
            if (mGestationTime<0.0f)
            {
                this->giveBirth();
            }
        }

        return true;
    }

    void Body::giveBirth()
    {

    }

    std::vector<std::shared_ptr<Item>> Body::heldItems() const
    {
        return mHeldItems;
    }

    bool Body::hasItem(std::shared_ptr<Item> item) const
    {
        auto iter=std::find_if(std::begin(mHeldItems),std::end(mHeldItems),[&item](std::shared_ptr<Item> const& value)
        {
            return item->ID()==value->ID();
        });

        return (iter!=std::end(mHeldItems));
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
        for (auto section : sectionList)
        {
            if (section->canCarry())
                return true;
        }

        return false;
    }

    bool Body::canCarry(EquipmentType equipType) const
    {
        auto iter=std::find_if(std::begin(mEquipmentSlots),std::end(mEquipmentSlots),[&equipType](BodySectionInfo const& value)
        {
            return equipType==value.type;
        });

        if (iter==std::end(mEquipmentSlots))
            return false;

        return this->canCarry(iter->sections);
    }

    bool Body::canCarry(std::shared_ptr<Item> item) const
    {
        if (!this->canCarry(EquipmentType::LeftHand))
            return this->canCarry(EquipmentType::RightHand);

        return true;
    }

    std::vector<std::shared_ptr<Item>> Body::getDropedItem(EquipmentType equipType)
    {
        std::vector<std::shared_ptr<Item>> result;
        if ((equipType!=EquipmentType::LeftHand) && (equipType!=EquipmentType::RightHand))
            return result;

        auto iter=std::find_if(std::begin(mEquipmentSlots),std::end(mEquipmentSlots),[&equipType](BodySectionInfo const& value)
        {
            return value.type==equipType;
        });

        if (iter==std::end(mEquipmentSlots))
            return result;

        for (auto section : iter->sections)
        {
            auto heldItem=section->heldItem();
            if (heldItem!= nullptr)
            {
                if (this->dropItem(heldItem))
                    result.push_back(heldItem);
            }
        }

        return result;
    }

    bool Body::sheathItem()
    {
        auto iter=std::find_if(std::begin(mAdditionalEquipmentSlots),std::end(mAdditionalEquipmentSlots),[](EquipItemInfo const& value)
        {
            return value.type==EquipmentType::Belt;
        });

        if (iter!=std::end(mAdditionalEquipmentSlots))
            return false;

        std::vector<std::shared_ptr<Item>> objects;
        if (!this->canCarry(EquipmentType::RightHand))
        {
            auto items=this->getDropedItem(EquipmentType::RightHand);
            for (auto item : items)
                objects.push_back(item);
        }

        if ((objects.size()==0) && (!this->canCarry(EquipmentType::LeftHand)))
        {
            auto items=this->getDropedItem(EquipmentType::LeftHand);
            for (auto item : items)
                objects.push_back(item);
        }

        if (objects.size()==0)
            return false;

        EquipItemInfo new_add_slot;
        new_add_slot.type=EquipmentType::Belt;
        new_add_slot.item=objects.at(0);
        mAdditionalEquipmentSlots.push_back(new_add_slot);

        auto object_id=objects.at(0)->ID();
        auto iter_equip=std::find_if(std::begin(mEquippedItems),std::end(mEquippedItems),[&object_id](std::shared_ptr<Item> const& value)
        {
            return value->ID()==object_id;
        });

        if (iter_equip==std::end(mEquippedItems))
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
        this->dropItem(item);

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
}