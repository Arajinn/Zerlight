//
// Created by tatiana on 26.06.2018.
//

#ifndef ZERL_FINDFOOD_H
#define ZERL_FINDFOOD_H

#include "game/behavior/Sequence.h"

namespace behavior {
    class FindFood : public Sequence{
    public:
        FindFood();
        ~FindFood();

        static TaskResult eatLikeABarbarian(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERL_FINDFOOD_H
