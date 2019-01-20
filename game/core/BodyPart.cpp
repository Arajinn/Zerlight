//
// Created by tatiana on 6/14/2018.
//

#include "BodyPart.h"
#include "BodySection.h"
#include "Body.h"
#include "Attack.h"
#include "Item.h"
#include "ItemHistory.h"
#include "Character.h"
#include "CharacterHistory.h"
#include "Weapon.h"
#include "Enums2Str.h"
#include "String2Enums.h"
#include "defines.h"
#include "game/properties/BodyPartDef.h"
#include "game/properties/DamageDef.h"
#include "game/properties/NaturalWeaponDef.h"
#include "game/properties/RaceDefinition.h"
#include "game/properties/WeaponDef.h"
#include "game/properties/AttackDef.h"
#include "game/utils/Randomizer.h"

#include <algorithm>

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
        std::shared_ptr<BodyPart> ptr=std::make_shared<BodyPart>();
        ptr->init(section,aBodyPartDef,nameModifier,containedBy,prostheticPart);
        return ptr;
    }

    void BodyPart::init(std::shared_ptr<game::BodySection> section,std::shared_ptr<const properties::BodyPartDef> aBodyPartDef,
            std::string nameModifier, std::shared_ptr<game::BodyPart> containedBy,std::shared_ptr<game::Item> prostheticPart)
    {
        mName=nameModifier+(!nameModifier.empty() ? " " : "")+aBodyPartDef->Name;

        parent_section=section;
        mProstheticPart=prostheticPart;

        std::string materialID=aBodyPartDef->MaterialID;
        std::string newMaterialID;
        if (aBodyPartDef->TemplateMaterialIndex>=0)
        {
            if (mProstheticPart!=nullptr)
                newMaterialID=mProstheticPart->history()->materialAtIndex(aBodyPartDef->TemplateMaterialIndex);
            else
                newMaterialID=section->body()->character()->history()->materialAtIndex(aBodyPartDef->TemplateMaterialIndex);
        }
        if (!newMaterialID.empty())
            materialID=newMaterialID;

        mMaterial=GAME_DEFINITIONS->materialDefinition(materialID);

        mNaturalWeapon=nullptr;
        if (aBodyPartDef->NaturalWeapon!=nullptr)
        {
            if (aBodyPartDef->NaturalWeapon->TemplateMaterialIndex>=0)
            {
                auto naturalWeaponDef=std::make_shared<properties::NaturalWeaponDef>(*aBodyPartDef->NaturalWeapon);

                if (mProstheticPart!=nullptr)
                    materialID=mProstheticPart->history()->materialAtIndex(aBodyPartDef->NaturalWeapon->TemplateMaterialIndex);
                else
                    materialID=section->body()->character()->history()->materialAtIndex(aBodyPartDef->NaturalWeapon->TemplateMaterialIndex);

                naturalWeaponDef->MaterialID=materialID;
                mNaturalWeapon=std::make_shared<Weapon>(naturalWeaponDef);
            }
            else
                mNaturalWeapon=std::make_shared<Weapon>(aBodyPartDef->NaturalWeapon);

            parent_section->addWeapon(mNaturalWeapon);
        }

        mDamageTaken=0.0f;
        mBodyFunction=aBodyPartDef->BodyFunction;

        if (mProstheticPart==nullptr)
            addFunction();
        else
            regainFunction();

        mBodyProperties=aBodyPartDef->BodyProperties;
        mToHitWeight=aBodyPartDef->ToHitWeight;

        parent_contained=containedBy;
        mBodyPartDef=aBodyPartDef;
        for (const auto& containedPartDef : aBodyPartDef->ContainedParts)
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

        mStatus.push_back(BodyPartStatus::Good);
    }

    void BodyPart::pre_delete()
    {
        parent_section=nullptr;
    }

    const BodyFunction& BodyPart::function() const
    {
        return mBodyFunction;
    }

    void BodyPart::addFunction()
    {
        parent_section->addFunction(shared_from_this());
    }

    void BodyPart::regainFunction()
    {
        parent_section->regainFunction(shared_from_this());
    }

    void BodyPart::damageFunction()
    {
        parent_section->damageFunction(shared_from_this());
    }

    bool BodyPart::hasStatus(const BodyPartStatus& status) const
    {
        auto iter=std::find_if(mStatus.begin(),mStatus.end(),[&status](BodyPartStatus const& value)
        {
            return status==value;
        });

        return (iter!=mStatus.end());
    }

    void BodyPart::addStatus(const BodyPartStatus& status)
    {
        const auto iter=std::find_if(mStatus.begin(),mStatus.end(),[&status](BodyPartStatus const& value)
        {
            return status==value;
        });

        if (iter==mStatus.end())
            mStatus.push_back(status);
    }

    bool BodyPart::hasFunction(const BodyFunction& func) const
    {
        if ((hasStatus(BodyPartStatus::Disabled)) || (hasStatus(BodyPartStatus::Missing)))
            return false;

        if (mBodyFunction==func)
            return true;

        for (const auto& contained : mContainedParts)
        {
            if (contained->hasFunction(func))
                return true;
        }

        return false;
    }

    bool BodyPart::hasBodyProperty(const BodyPartProperty& property) const
    {
        auto iter=std::find_if(mBodyProperties.begin(),mBodyProperties.end(),[&property](BodyPartProperty const& elem)
        {
            return property==elem;
        });

        return (iter!=mBodyProperties.end());
    }

    std::shared_ptr<const properties::MaterialDef> BodyPart::material() const
    {
        return mMaterial;
    }

    bool BodyPart::isSevered() const
    {
        if (!hasStatus(BodyPartStatus::Severed))
            return false;

        for (const auto& part : mContainedParts)
        {
            if (!part->isSevered())
                return false;
        }

        return true;
    }

    bool BodyPart::isBleeding() const
    {
        if (hasStatus(BodyPartStatus::Bleeding))
            return true;

        for (const auto& part : mContainedParts)
        {
            if (part->isBleeding())
                return true;
        }

        return false;
    }

    bool BodyPart::hasInternalBleeding() const
    {
        if (hasStatus(BodyPartStatus::Bleeding))
            return false;

        for (const auto& part : mContainedParts)
        {
            if (part->isBleeding())
                return true;
        }

        return false;
    }

    bool BodyPart::isArteryStruck() const
    {
        if (hasStatus(BodyPartStatus::StruckArtery))
            return true;

        for (const auto& part : mContainedParts)
        {
            if (part->isArteryStruck())
                return true;
        }

        return false;
    }

    bool BodyPart::isCompletelyDestroyed() const
    {
        if (!hasStatus(BodyPartStatus::Disabled))
            return false;

        for (const auto& part : mContainedParts)
        {
            if (!part->isCompletelyDestroyed())
                return false;
        }

        return true;
    }

    size_t BodyPart::bleedingCount() const
    {
        size_t result=0;
        if (hasStatus(BodyPartStatus::Bleeding))
            result++;

        for (const auto& bodyPart : mContainedParts)
            result+=bodyPart->bleedingCount();

        return result;
    }

    size_t BodyPart::damagedArteriesCount() const
    {
        size_t result=0;
        if (hasStatus(BodyPartStatus::StruckArtery))
            result++;

        for (const auto& bodyPart : mContainedParts)
            result+=bodyPart->damagedArteriesCount();

        return result;
    }

    float BodyPart::toHitWeight() const
    {
        return mToHitWeight;
    }

    std::shared_ptr<BodyPart> BodyPart::randomContainedPart() const
    {
        float summ=0.0f;
        for (const auto& part : mContainedParts)
        {
            if ((!part->isSevered()) && (!part->isCompletelyDestroyed()))
                summ+=part->toHitWeight();
        }

        const float rand=RANDOMIZER->uniform(0.0f,1.0f)*summ;
        float value=0.0f;
        for (const auto& part : mContainedParts)
        {
            if ((!part->isSevered()) && (!part->isCompletelyDestroyed()))
            {
                value+=part->toHitWeight();

                if (rand<value)
                    return part;
            }
        }

        return nullptr;
    }

    void BodyPart::containedPartDisabled(std::shared_ptr<BodyPart> containedPart)
    {
        const auto function=containedPart->function();
        if ((function!=BodyFunction::Structure) && (function!=BodyFunction::Motor))
            return;

        if ((mBodyFunction==BodyFunction::Motor) || (mBodyFunction==BodyFunction::Grip)
            || (mBodyFunction==BodyFunction::Stand) || (mBodyFunction==BodyFunction::Wing))
        {
            lostFunction();

            if (parent_contained!=nullptr)
                parent_contained->containedPartDisabled(shared_from_this());
        }
    }

    std::shared_ptr<BodySection> BodyPart::section() const
    {
        return parent_section;
    }

    void BodyPart::dependantPartLost(std::shared_ptr<BodyPart> bodyPart)
    {
        if ((mBodyFunction==BodyFunction::Grip) || (mBodyFunction==BodyFunction::Stand) || (mBodyFunction==BodyFunction::Wing))
            lostFunction();

        for (const auto& containPart : mContainedParts)
            containPart->dependantPartLost(bodyPart);
    }

    void BodyPart::dependantPartDamaged(std::shared_ptr<BodyPart> bodyPart)
    {
        if ((mBodyFunction==BodyFunction::Grip) || (mBodyFunction==BodyFunction::Stand) || (mBodyFunction==BodyFunction::Wing))
            damageFunction();

        for (const auto& containPart : mContainedParts)
            containPart->dependantPartDamaged(bodyPart);
    }

    void BodyPart::lostFunction()
    {
        if (hasStatus(BodyPartStatus::Disabled))
            return;

        mStatus.push_back(BodyPartStatus::Disabled);

        if (mNaturalWeapon!=nullptr)
            parent_section->removeWeapon(mNaturalWeapon);

        if (parent_contained!=nullptr)
            parent_contained->containedPartDisabled(shared_from_this());

        parent_section->lostFunction(shared_from_this());
    }

    std::string BodyPart::name() const
    {
        return mName;
    }

    void BodyPart::damage(std::shared_ptr<Attack> attack, size_t damageTypeIndex, std::vector<std::string>& hitEffects)
    {
        std::cout << "damage " + mName << std::endl;

        const auto attackDamage=attack->damage(damageTypeIndex);
        const auto damageType=attackDamage.Type;
        const auto damageValue=attackDamage.Amount;

        const auto damageDef=mMaterial->DamageProperties.at((size_t)damageType);
        float value=mMaterial->Sustains*mBodyPartDef->Thickness;
        if (mDamageTaken>=value)
        {
            const auto bodyPart=randomContainedPart();
            if (bodyPart==nullptr)
                return;

            bodyPart->damage(attack,damageTypeIndex,hitEffects);
        }
        else if ((damageValue>0.0f) && (damageValue>=damageDef->Blocks))
        {
            const float delta=(damageValue-damageDef->Blocks)*damageDef->PercentReceived;
            mDamageTaken+=delta;

            if (delta>=damageDef->Break)
                mDamageTaken=value+delta;

            if (mDamageTaken>=value)
            {
                lostFunction();

                if (damageType==DamageType::Heat)
                    addStatus(BodyPartStatus::Missing);

                if (damageType==DamageType::Slash)
                    addStatus(BodyPartStatus::Severed);

                if ((damageType==DamageType::Pierce) || (damageType==DamageType::Slash))
                {
                    if (hasBodyProperty(BodyPartProperty::HasBlood))
                        addStatus(BodyPartStatus::Bleeding);

                    if (hasBodyProperty(BodyPartProperty::HasArtery))
                        addStatus(BodyPartStatus::StruckArtery);
                }

                if ((attack->statusEffect()==WeaponStatusEffectType::ZombieVirus) && (hasBodyProperty(BodyPartProperty::HasBlood))
                    && (parent_section->body()->character()->raceDefinition()->ZombieVirusCarrier))
                {
                    parent_section->body()->addStatusEffect(HealthStatusAilment::ZombieVirus,1.0f,0.0f,true);
                }

                auto bodyPart=randomContainedPart();
                if ((damageType==DamageType::Blunt) && (bodyPart!=nullptr) && (hasBodyProperty(BodyPartProperty::Splinters)))
                {
                    hitEffects.push_back("splintering the "+mName);

                    std::shared_ptr<properties::AttackDef> attackDef=std::make_shared<properties::AttackDef>();
                    attackDef->Verb="puncturing";
                    attackDef->AttackType=AttackType::Thrust;
                    attackDef->DamageTypes.emplace_back(DamageType::Pierce);

                    std::shared_ptr<properties::WeaponDef> weaponDef=std::make_shared<properties::WeaponDef>();
                    weaponDef->Point=0.5f;
                    weaponDef->AttackMoves.push_back(attackDef);

                    std::shared_ptr<properties::NaturalWeaponDef> naturalWeapon=std::make_shared<properties::NaturalWeaponDef>();
                    naturalWeapon->Name=mName;
                    naturalWeapon->Size=100.0f;
                    naturalWeapon->MaterialID=mMaterial->ID;
                    naturalWeapon->weaponDef=weaponDef;

                    std::shared_ptr<Weapon> weapon=std::make_shared<Weapon>(naturalWeapon);
                    std::shared_ptr<Attack> naturalAttack=std::make_shared<Attack>(weapon,attackDef,1.0f,1.0f);

                    hitEffects.push_back(attackDef->Verb+" the "+bodyPart->name());
                    bodyPart->damage(naturalAttack,0,hitEffects);
                }
                else
                {
                    hitEffects.push_back(properties::AttackDef::destroyString(attack->damage(damageTypeIndex).Type)+" the "+mName);
                }

                if (bodyPart==nullptr)
                    return;

                attack->setDamage(damageTypeIndex,mDamageTaken-value);
                bodyPart->damage(attack,damageTypeIndex,hitEffects);
            }
            else
            {
                if ((damageType==DamageType::Pierce) || (damageType==DamageType::Slash))
                {
                    if (hasBodyProperty(BodyPartProperty::HasBlood))
                        addStatus(BodyPartStatus::Bleeding);

                    if (hasBodyProperty(BodyPartProperty::HasArtery))
                        addStatus(BodyPartStatus::StruckArtery);
                }

                if ((attack->statusEffect()==WeaponStatusEffectType::ZombieVirus) && (hasBodyProperty(BodyPartProperty::HasBlood))
                    && (parent_section->body()->character()->raceDefinition()->ZombieVirusCarrier))
                {
                    parent_section->body()->addStatusEffect(HealthStatusAilment::ZombieVirus,1.0f,0.0f,true);
                }

                damageFunction();

                hitEffects.push_back(properties::AttackDef::damageString(attack->damage(damageTypeIndex).Type)+" the "+mName);

                if (damageDef->PercentTransfered>0.0f)
                {
                    auto bodyPart=randomContainedPart();
                    if (bodyPart==nullptr)
                        return;

                    attack->setDamage(damageTypeIndex,(damageValue-damageDef->Blocks)*damageDef->PercentTransfered);
                    bodyPart->damage(attack,damageTypeIndex,hitEffects);
                }
                else
                {
                    damageTypeIndex++;
                    if (damageTypeIndex>=attack->damageCount())
                        return;

                    damage(attack,damageTypeIndex,hitEffects);
                }
            }
        }
        else
        {
            damageTypeIndex++;
            if (damageTypeIndex<attack->damageCount())
                damage(attack,damageTypeIndex,hitEffects);
            else
                hitEffects.push_back("stopping at the "+mName);

            for (const auto& hitEffect : hitEffects)
                std::cout << hitEffect << std::endl;
        }
    }

    void BodyPart::damageFunctionThroughArmor(const float& value)
    {
        const auto damageDef=mMaterial->DamageProperties.at(2);
        const float amount=mMaterial->Sustains*mBodyPartDef->Thickness;

        if (mDamageTaken>=amount)
        {
            const auto bodyPart=randomContainedPart();
            if (bodyPart==nullptr)
                return;

            bodyPart->damageFunctionThroughArmor(value);
        }
        else
        {
            if (value<=0.0f)
                return;

            if (value<damageDef->Blocks)
                return;

            const float delta=value-damageDef->Blocks;

            damageFunction();

            if (damageDef->PercentTransfered>0.0f)
            {
                const auto bodyPart=randomContainedPart();
                if (bodyPart==nullptr)
                    return;

                bodyPart->damageFunctionThroughArmor(delta*damageDef->PercentTransfered);
            }
            else
            {
                const float getDamage=delta*damageDef->PercentReceived;
                if (getDamage<=amount)
                    return;

                const auto bodyPart=randomContainedPart();
                if (bodyPart==nullptr)
                    return;

                bodyPart->damageFunctionThroughArmor(getDamage-amount);
            }
        }
    }

    void BodyPart::disable()
    {
        lostFunction();

        for (const auto& bodyPart : mContainedParts)
            bodyPart->disable();
    }

    std::shared_ptr<Item> BodyPart::prostheticPart() const
    {
        return mProstheticPart;
    }

    std::shared_ptr<Item> BodyPart::removeProstheticPart()
    {
        if (mProstheticPart==nullptr)
            return nullptr;

        lostFunction();

        for (const auto& bodyPart : mContainedParts)
            bodyPart->removeProstheticPart();

        if (std::dynamic_pointer_cast<Character>(mProstheticPart->parent())!=nullptr)
            mProstheticPart->setParent(nullptr);

        return mProstheticPart;
    }

    void BodyPart::healWound()
    {
        if (hasStatus(BodyPartStatus::Missing))
            return;

        if (hasStatus(BodyPartStatus::Disabled))
        {
            if (mNaturalWeapon!=nullptr)
                parent_section->addWeapon(mNaturalWeapon);

            regainFunction();
        }

        mStatus.clear();
        addStatus(BodyPartStatus::Good);
        mDamageTaken=0.0f;

        for (const auto& bodyPart : mContainedParts)
            bodyPart->healWound();
    }
}