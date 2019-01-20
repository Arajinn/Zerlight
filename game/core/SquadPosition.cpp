//
// Created by tatiana on 26.03.2018.
//

#include "SquadPosition.h"

namespace game
{
    SquadPosition::SquadPosition()
    {
        mPerk=SquadPositionPerk::None;
        mAttackSquadLeaderTarget=false;
        mStayAtMaxRange=false;
        mRetreatWhenBleeding=false;
        mRetreatToEquip=false;
        mRetreatToFillAmmo=false;
        mPursueEnemies=true;
    }

    SquadPosition::~SquadPosition()
    {

    }

    const SquadPositionPerk& SquadPosition::perk() const
    {
        return mPerk;
    }

    const bool& SquadPosition::attackSquadLeaderTarget() const
    {
        return mAttackSquadLeaderTarget;
    }

    const bool& SquadPosition::stayAtMaxRange() const
    {
        return mStayAtMaxRange;
    }

    const bool& SquadPosition::retreatWhenBleeding() const
    {
        return mRetreatWhenBleeding;
    }

    const bool& SquadPosition::retreatToEquip() const
    {
        return mRetreatToEquip;
    }

    const bool& SquadPosition::retreatToFillAmmo() const
    {
        return mRetreatToFillAmmo;
    }

    const bool& SquadPosition::pursueEnemies() const
    {
        return mPursueEnemies;
    }
}