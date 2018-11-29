//
// Created by tatiana.polozova on 26.06.2018.
//

#ifndef ZERL_LOOKINGFORFOOD_H
#define ZERL_LOOKINGFORFOOD_H

#include "game/behavior/Sequence.h"

namespace behavior {
    class LookingForFood : public Sequence{
    public:
        LookingForFood();
        ~LookingForFood();

        static TaskResult isLookingForFood(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERL_LOOKINGFORFOOD_H
