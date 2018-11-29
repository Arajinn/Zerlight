//
// Created by tatiana.polozova on 26.03.2018.
//

#ifndef ZERL_FORMATION_H
#define ZERL_FORMATION_H

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

        friend class Squad;
    private:
        std::vector<std::shared_ptr<SquadPosition>> positions;
    };
}

#endif //ZERL_FORMATION_H
