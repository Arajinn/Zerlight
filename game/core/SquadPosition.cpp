//
// Created by tatiana.polozova on 26.03.2018.
//

#include "SquadPosition.h"

namespace game
{
    SquadPosition::SquadPosition()
    {

    }

    SquadPosition::~SquadPosition()
    {

    }

    const SquadPositionPerk& SquadPosition::perk() const
    {
        return mPerk;
    }
}