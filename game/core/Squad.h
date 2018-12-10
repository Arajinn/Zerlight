//
// Created by tatiana on 26.03.2018.
//

#ifndef ZERL_SQUAD_H
#define ZERL_SQUAD_H

#include <memory>
#include <vector>

namespace game
{
    class Character;
    class Formation;
    class SquadPosition;
}

namespace game {
    class Squad {
    public:
        Squad();
        ~Squad();

        std::shared_ptr<SquadPosition> position(const Character* member);
        float formationMoveSpeedBonus() const;
    private:
        std::vector<std::shared_ptr<Character>> mMembers;
        std::shared_ptr<Formation> mFormation;

        unsigned int positionIndex(const Character* member);
        std::shared_ptr<SquadPosition> positionByIndex(unsigned int posIndex);
    };
}

#endif //ZERL_SQUAD_H
