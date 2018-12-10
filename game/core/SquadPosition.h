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
    private:
        SquadPositionPerk mPerk;
    };
}

#endif //ZERL_SQUADPOSITION_H
