//
// Created by tatiana on 26.12.18.
//

#ifndef ZERLIGHT_ATTACK_H
#define ZERLIGHT_ATTACK_H

#include "game/core/enums.h"

#include <vector>
#include <memory>

namespace properties
{
    struct AttackDef;
}

namespace game
{
    class Weapon;
}

namespace game {
    struct Damage
    {
        Damage();
        Damage(DamageType aType,float aAmount);

        DamageType Type;
        float Amount;
    };

    class Attack {
    public:
        Attack(std::shared_ptr<Weapon> aWeapon,std::shared_ptr<const properties::AttackDef> aAttackDef,
                const float& baseDamage, const float& velocity);
        ~Attack();

        std::shared_ptr<const properties::AttackDef> attackDef() const;
        std::shared_ptr<Weapon> weapon() const;

        Damage damage(const size_t& index) const;
        void setDamage(const size_t& index,const float& amount);
        size_t damageCount() const;

        WeaponStatusEffectType statusEffect() const;
    private:
        std::shared_ptr<Weapon> mWeapon;
        std::shared_ptr<const properties::AttackDef> mAttackDef;
        std::vector<Damage> mDamages;
        WeaponStatusEffectType mStatusEffect;
    };
}

#endif //ZERLIGHT_ATTACK_H
