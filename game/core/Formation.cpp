//
// Created by tatiana on 26.03.2018.
//

#include "Formation.h"

namespace game
{
    Formation::Formation()
    {
        mAvoidEnemies=false;
        mPerk=FormationPerk::None;
        mDefendGnomes=false;
    }

    Formation::~Formation()
    {

    }

    const bool& Formation::avoidEnemies() const
    {
        return mAvoidEnemies;
    }

    const bool& Formation::defendGnomes() const
    {
        return mDefendGnomes;
    }

    std::vector<std::shared_ptr<SquadPosition>> Formation::positions() const
    {
        return mPositions;
    }

    FormationPerk Formation::perk() const
    {
        return mPerk;
    }
}