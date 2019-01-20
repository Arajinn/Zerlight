//
// Created by tatiana on 23.12.18.
//

#include "AttackDef.h"

namespace properties
{
    AttackDef::AttackDef()
    {

    }

    AttackDef::~AttackDef()
    {

    }

    bool AttackDef::isPhysicalAttack() const
    {
        if ((AttackType==game::AttackType::Swing) || (AttackType==game::AttackType::Thrust) || (AttackType==game::AttackType::HandToHand)
        || (AttackType==game::AttackType::Projectile))
            return true;
        else
            return false;
    }

    bool AttackDef::isRangedAttack() const
    {
        if (AttackType==game::AttackType::Projectile)
            return true;
        else
            return false;
    }

    std::string AttackDef::damageString(const game::DamageType& damageType)
    {
        return "damage";
    }

    std::string AttackDef::destroyString(const game::DamageType& damageType)
    {
        return "destroy";
    }
}