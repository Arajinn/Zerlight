//
// Created by tatiana on 23.12.18.
//

#ifndef ZERLIGHT_ATTACKDEF_H
#define ZERLIGHT_ATTACKDEF_H

#include "game/core/enums.h"

#include <vector>
#include <string>

namespace properties {
    struct AttackDef {
        AttackDef();
        ~AttackDef();

        float AttackRange;
        bool RequiresAmmo;
        int MinimumSkillLevel;
        float Weight;
        float DamageScale;
        std::vector<game::DamageType> DamageTypes;
        float AttackTime;

        game::AttackType AttackType;

        bool isPhysicalAttack() const;
        bool isRangedAttack() const;

        std::string Verb;

        static std::string damageString(const game::DamageType& damageType);
        static std::string destroyString(const game::DamageType& damageType);
    };
}

#endif //ZERLIGHT_ATTACKDEF_H
