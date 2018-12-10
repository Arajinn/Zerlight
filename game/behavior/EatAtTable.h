//
// Created by tatiana on 26.06.2018.
//

#ifndef ZERL_EATATTABLE_H
#define ZERL_EATATTABLE_H

#include "game/behavior/Sequence.h"

namespace behavior {
    class EatAtTable : public Sequence{
    public:
        EatAtTable();
        ~EatAtTable();

        static TaskResult wait(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult eatInComfort(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERL_EATATTABLE_H
