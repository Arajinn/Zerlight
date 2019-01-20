//
// Created by tatiana on 26.12.18.
//

#include "Attack.h"
#include "defines.h"
#include "Weapon.h"
#include "game/properties/WeaponDef.h"
#include "game/properties/AttackDef.h"
#include "game/properties/ItemSettings.h"
#include "game/properties/AmmoDef.h"

namespace game
{
    Damage::Damage()
    {

    }

    Damage::Damage(DamageType aType,float aAmount)
    {
        Type=aType;
        Amount=aAmount;
    }

    Attack::Attack(std::shared_ptr<Weapon> aWeapon,std::shared_ptr<const properties::AttackDef> aAttackDef,
        const float& baseDamage, const float& velocity)
    {
        mWeapon=aWeapon;
        mAttackDef=aAttackDef;
        mStatusEffect=aWeapon->weaponDef()->StatusEffect;
        const float scaleDamage=baseDamage*aAttackDef->DamageScale;
        if (aAttackDef->RequiresAmmo)
        {
            const auto ammoID=ITEM_SETTINGS->itemIDToAmmoID(aWeapon->weaponDef()->AmmoItemID);
            const auto ammoDef=GAME_DEFINITIONS->ammoDefinition(ammoID);
            const auto damages=ammoDef->DamageTypes;
            for (const auto& damageType : damages)
            {
                mDamages.emplace_back(Damage(damageType,scaleDamage*aWeapon->calculateProjectileDamage(ammoDef,damageType)));
            }
        }
        else
        {
            const auto damages=aAttackDef->DamageTypes;
            for (const auto damageType : damages)
            {
                if (aAttackDef->isPhysicalAttack())
                    mDamages.emplace_back(Damage(damageType,scaleDamage*aWeapon->calculatePhysicalDamage(damageType,velocity)));
                else
                    mDamages.emplace_back(Damage(damageType,scaleDamage));
            }
        }
    }

    Attack::~Attack()
    {

    }

    std::shared_ptr<const properties::AttackDef> Attack::attackDef() const
    {
        return mAttackDef;
    }

    std::shared_ptr<Weapon> Attack::weapon() const
    {
        return mWeapon;
    }

    Damage Attack::damage(const size_t& index) const
    {
        return mDamages.at(index);
    }

    void Attack::setDamage(const size_t& index,const float& amount)
    {
        mDamages[index].Amount=amount;
    }

    WeaponStatusEffectType Attack::statusEffect() const
    {
        return mStatusEffect;
    }

    size_t Attack::damageCount() const
    {
        return mDamages.size();
    }
}