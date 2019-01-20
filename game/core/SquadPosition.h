//
// Created by tatiana on 26.03.2018.
//

#ifndef ZERL_SQUADPOSITION_H
#define ZERL_SQUADPOSITION_H

#include "game/core/enums.h"

namespace game {
    class SquadPosition {
    public:
        SquadPosition();
        ~SquadPosition();

        const SquadPositionPerk& perk() const;

        const bool& attackSquadLeaderTarget() const;
        const bool& stayAtMaxRange() const;
        const bool& retreatWhenBleeding() const;
        const bool& retreatToEquip() const;
        const bool& retreatToFillAmmo() const;
        const bool& pursueEnemies() const;
    private:
        SquadPositionPerk mPerk;
        bool mAttackSquadLeaderTarget;
        bool mStayAtMaxRange;
        bool mRetreatWhenBleeding;
        bool mRetreatToEquip;
        bool mRetreatToFillAmmo;
        bool mPursueEnemies;
    };
}

#endif //ZERL_SQUADPOSITION_H
