//
// Created by tatiana on 23.12.18.
//

#include "Weapon.h"
#include "defines.h"
#include "Character.h"
#include "SquadPosition.h"
#include "BodySection.h"
#include "BodyPart.h"
#include "Body.h"
#include "Item.h"
#include "StorageContainer.h"
#include "Enums2Str.h"
#include "game/properties/WeaponDef.h"
#include "game/properties/AttackDef.h"
#include "game/properties/TargetedAttackDef.h"
#include "game/properties/AmmoDef.h"
#include "game/properties/DefendDef.h"
#include "game/properties/NaturalWeaponDef.h"
#include "game/properties/ItemDefinition.h"
#include "game/utils/Randomizer.h"

namespace game {
    Weapon::Weapon(std::shared_ptr<const properties::NaturalWeaponDef> naturalWeapon)
    {
        mName=naturalWeapon->Name;
        mSize=naturalWeapon->Size;
        mMaterialID=naturalWeapon->MaterialID;
        mWeaponDef=naturalWeapon->weaponDef;
        mItem=nullptr;
        mAttackTimer=0.0f;
        mDefendTimer=0.0f;
    }

    Weapon::Weapon(std::shared_ptr<Item> item)
    {
        mName=item->name();
        const auto itemDef=GAME_DEFINITIONS->itemDefinition(item->itemID());
        mSize=itemDef->WeaponSize;
        mMaterialID=item->materialID();
        mWeaponDef=itemDef->ItemWeaponDef;
        mItem=item;
        mAttackTimer=0.0f;
        mDefendTimer=0.0f;
    }

    Weapon::~Weapon()
    {

    }

    std::shared_ptr<const properties::WeaponDef> Weapon::weaponDef() const
    {
        return mWeaponDef;
    }

    bool Weapon::withinRange(const float& distance, std::shared_ptr<const Character> owner) const
    {
        const auto squad_pos=owner->squadPosition();
        const auto attackMoves=mWeaponDef->AttackMoves;
        for (const auto attackMove : attackMoves)
        {
            float range=attackMove->AttackRange;
            if ((squad_pos!= nullptr) && (squad_pos->perk()==SquadPositionPerk::Marksgnome) && (range>2.0f))
                range*=2.25f;

            if ((range>=distance) && ((!attackMove->RequiresAmmo) || (owner->hasAmmo(shared_from_this()))))
                return true;
        }

        return false;
    }

    float Weapon::maxRange(std::shared_ptr<const Character> owner) const
    {
        float result=1.0f;
        const auto squad_pos=owner->squadPosition();
        for (const auto attackMove : mWeaponDef->AttackMoves)
        {
            float range=attackMove->AttackRange;
            if ((squad_pos!= nullptr) && (squad_pos->perk()==SquadPositionPerk::Marksgnome) && (range>2.0f))
                range*=2.25f;

            if ((range>result) && ((!attackMove->RequiresAmmo) || (owner->hasAmmo(shared_from_this()))))
                result=range;
        }

        return result;
    }

    bool Weapon::canAttack() const
    {
        return (mAttackTimer<=0.0f);
    }

    void Weapon::delayAttack(const float& delay)
    {
        mAttackTimer+=delay;
    }

    std::shared_ptr<properties::AttackDef> Weapon::weightedAttackMove(std::vector<std::shared_ptr<properties::AttackDef>> attackMoves) const
    {
        float summWeight=0.0f;
        for (const auto& attackMove : attackMoves)
            summWeight+=attackMove->Weight;

        const float randWeight=RANDOMIZER->uniform()*summWeight;
        float weight=0.0f;
        for (const auto& attackMove : attackMoves)
        {
            weight+=attackMove->Weight;
            if (weight>=randWeight)
                return attackMove;
        }

        return nullptr;
    }

    std::shared_ptr<properties::AttackDef> Weapon::randomAttackInRange(float distance,std::shared_ptr<const Character> owner) const
    {
        const auto squad_pos=owner->squadPosition();
        std::vector<std::shared_ptr<properties::AttackDef>> results;
        for (const auto attackMove : mWeaponDef->AttackMoves)
        {
            float range=attackMove->AttackRange;
            if ((squad_pos!= nullptr) && (squad_pos->perk()==SquadPositionPerk::Marksgnome) && (range>2.0))
                range*=2.25f;
//TODO if ((attackMove->AttackRange<=distance) && (range>=distance)
            if ((range>=distance)
            && (owner->skillLevel(mWeaponDef->Skill)>=attackMove->MinimumSkillLevel)
            && ((!attackMove->RequiresAmmo) || (owner->hasAmmo(shared_from_this()))))
                results.push_back(attackMove);
        }

        return weightedAttackMove(results);
    }

    std::shared_ptr<properties::AttackDef> Weapon::bestAttackInRange(float distance,std::shared_ptr<const Character> owner,
                                                             std::shared_ptr<const BodySection> sectionHit) const
    {
        const auto squad_pos=owner->squadPosition();
        auto materialProperty=sectionHit->bodyPart()->material();

        if (sectionHit->equippedItem()!=nullptr)
            materialProperty=GAME_DEFINITIONS->materialDefinition(sectionHit->equippedItem()->materialID());

        std::vector<std::shared_ptr<properties::AttackDef>> results;
        for (const auto& targetedAttackMove : mWeaponDef->TargetedAttackMoves)
        {
            if (targetedAttackMove->isContain(materialProperty->Type))
            {
                float range=targetedAttackMove->TargetedAttackDef->AttackRange;
                if ((squad_pos!= nullptr) && (squad_pos->perk()==SquadPositionPerk::Marksgnome) && (range>2.0))
                    range*=2.25f;

                if ((targetedAttackMove->TargetedAttackDef->AttackRange<=distance) && (range>=distance)
                && ((!targetedAttackMove->TargetedAttackDef->RequiresAmmo) || (owner->hasAmmo(shared_from_this())))
                && (owner->skillLevel(mWeaponDef->Skill)>=targetedAttackMove->TargetedAttackDef->MinimumSkillLevel))
                    results.push_back(targetedAttackMove->TargetedAttackDef);
            }
        }

        if (results.empty())
            return randomAttackInRange(distance,owner);
        else
            return weightedAttackMove(results);
    }

    std::shared_ptr<Item> Weapon::item() const
    {
        return mItem;
    }

    float Weapon::qualityModifier() const
    {
        if (mItem==nullptr)
            return 1.0f;

        const auto quality=mItem->quality();
        switch (quality)
        {
            case ItemQuality::Poor:
                return 0.9f;
            case ItemQuality::Fine:
                return 1.1f;
            case ItemQuality::Superior:
                return 1.2f;
            case ItemQuality::Masterful:
                return 1.4f;
            case ItemQuality::Legendary:
                return 1.8f;
            default:
                return 1.0f;
        }
    }

    float Weapon::calculateProjectileDamage(std::shared_ptr<const properties::AmmoDef> ammoDef, const DamageType& damageType) const
    {
        const auto owner=mItem->claimedBy();
        if (owner==nullptr)
            return 0.0f;

        const auto equiped=owner->body()->equippedItemsInSlot(EquipmentType::Back);
        if (equiped.empty())
            return 0.0f;

        const auto storageContainer=std::dynamic_pointer_cast<StorageContainer>(equiped.at(0));
        if (storageContainer==nullptr)
            return 0.0f;

        if (storageContainer->containedResourcesCount()==0)
            return 0.0f;

        const auto containedResource=storageContainer->containedResources().at(0);
        const auto ammoMaterial=GAME_DEFINITIONS->materialDefinition(containedResource->materialID());
        const auto weaponMaterial=GAME_DEFINITIONS->materialDefinition(mMaterialID);
        float strength=weaponMaterial->Strength;
        if (ammoMaterial->Type!=MaterialType::Stone)
            strength+=ammoMaterial->Strength;

        if (damageType==DamageType::Pierce)
            return ammoDef->WeaponSize*strength*mWeaponDef->ProjectileModifier*ammoDef->VelocityModifier*weaponMaterial->PierceModifier
                *ammoDef->Point*qualityModifier();

        if (damageType==DamageType::Slash)
            return ammoDef->WeaponSize*strength*mWeaponDef->ProjectileModifier*ammoDef->VelocityModifier*weaponMaterial->SlashModifier
                   *ammoDef->Edge*qualityModifier();

        if (damageType==DamageType::Blunt)
            return ammoDef->WeaponSize*strength*mWeaponDef->ProjectileModifier*ammoDef->VelocityModifier*weaponMaterial->BluntModifier
                   *ammoDef->BluntModifier*qualityModifier();

        return ammoDef->WeaponSize*weaponMaterial->Strength*mWeaponDef->ProjectileModifier*ammoDef->VelocityModifier*qualityModifier();
    }

    float Weapon::calculatePhysicalDamage(const DamageType& damageType, const float& velocity) const
    {
        const auto weaponMaterial=GAME_DEFINITIONS->materialDefinition(mMaterialID);

        if (damageType==DamageType::Pierce)
            return mSize*weaponMaterial->Strength*mWeaponDef->VelocityModifier*velocity*weaponMaterial->PierceModifier
                *mWeaponDef->Point*qualityModifier();

        if (damageType==DamageType::Slash)
            return mSize*weaponMaterial->Strength*mWeaponDef->VelocityModifier*velocity*weaponMaterial->SlashModifier
                   *mWeaponDef->Edge*qualityModifier();

        if (damageType==DamageType::Blunt)
            return mSize*weaponMaterial->Strength*mWeaponDef->VelocityModifier*velocity*weaponMaterial->BluntModifier
                   *mWeaponDef->BluntModifier*qualityModifier();

        return mSize*weaponMaterial->Strength*mWeaponDef->VelocityModifier*velocity*qualityModifier();
    }

    void Weapon::attack(std::shared_ptr<const properties::AttackDef> attackDef)
    {
        mAttackTimer+=attackDef->AttackTime;
    }

    std::string Weapon::name() const
    {
        if (mItem==nullptr)
            return mName;
        else
            return mItem->name();
    }

    bool Weapon::canDefend() const
    {
        if (mDefendTimer<=0.0f)
            return (!mWeaponDef->DefendMoves.empty());

        return false;
    }

    void Weapon::defend(std::shared_ptr<const properties::DefendDef> defendDef, float modifier)
    {
        mDefendTimer+=defendDef->DefendTime;
        delayAttack(defendDef->AttackDelay*modifier);
    }

    void Weapon::update(const float& dt)
    {
        if (mAttackTimer>0.0f)
            mAttackTimer-=dt;

        if (mDefendTimer>0.0f)
            mDefendTimer-=dt;
    }
}