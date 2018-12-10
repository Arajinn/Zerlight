//
// Created by tatiana on 26.06.2018.
//

#ifndef ZERL_SATISFYHUNGER_H
#define ZERL_SATISFYHUNGER_H

#include "game/behavior/Sequence.h"

namespace behavior {
    class SatisfyHunger : public Sequence{
    public:
        SatisfyHunger();
        ~SatisfyHunger();

        static TaskResult hungry(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERL_SATISFYHUNGER_H
