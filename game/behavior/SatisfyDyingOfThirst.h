//
// Created by tatiana on 6/15/2018.
//

#ifndef ZERL_SATISFYDYINGOFTHIRST_H
#define ZERL_SATISFYDYINGOFTHIRST_H

#include "game/behavior/Sequence.h"

namespace behavior {
    class SatisfyDyingOfThirst : public Sequence{
    public:
        SatisfyDyingOfThirst();
        ~SatisfyDyingOfThirst();

        static TaskResult dyingOfThirst(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERL_SATISFYDYINGOFTHIRST_H
