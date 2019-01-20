//
// Created by tatiana on 26.03.2018.
//

#ifndef ZERL_FORMATION_H
#define ZERL_FORMATION_H

#include "game/core/enums.h"

#include <vector>
#include <memory>

namespace game
{
    class SquadPosition;
}

namespace game {
    class Formation {
    public:
        Formation();
        ~Formation();

        const bool& avoidEnemies() const;
        const bool& defendGnomes() const;

        std::vector<std::shared_ptr<SquadPosition>> positions() const;

        FormationPerk perk() const;
    private:
        std::vector<std::shared_ptr<SquadPosition>> mPositions;
        bool mAvoidEnemies;
        FormationPerk mPerk;
        bool mDefendGnomes;
    };
}

#endif //ZERL_FORMATION_H
