//
// Created by tatiana on 23.12.18.
//

#ifndef ZERLIGHT_WEAPONDEF_H
#define ZERLIGHT_WEAPONDEF_H

#include "game/core/enums.h"

#include <string>
#include <vector>
#include <memory>

namespace properties
{
    struct AttackDef;
    struct TargetedAttackDef;
    struct DefendDef;
}

namespace properties {
    struct WeaponDef {
        std::string Skill;
        std::string AmmoItemID;

        std::vector<std::shared_ptr<AttackDef>> AttackMoves;
        std::vector<std::shared_ptr<DefendDef>> DefendMoves;
        std::vector<std::shared_ptr<TargetedAttackDef>> TargetedAttackMoves;

        game::WeaponStatusEffectType StatusEffect{game::WeaponStatusEffectType::None};

        float ProjectileModifier;
        float VelocityModifier;
        float BluntModifier;
        float KnockbackModifier;
        float Point;
        float Edge;

        std::vector<std::shared_ptr<DefendDef>> defendMoves(const game::AttackType& type) const;
    };
}

#endif //ZERLIGHT_WEAPONDEF_H
