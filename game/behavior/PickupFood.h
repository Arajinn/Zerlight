//
// Created by tatiana on 25.06.2018.
//

#ifndef ZERL_PICKUPFOOD_H
#define ZERL_PICKUPFOOD_H

#include "game/behavior/Sequence.h"

namespace behavior{
    class PickupFood : public Sequence{
    public:
        PickupFood();
        ~PickupFood();

        static TaskResult pickup(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERL_PICKUPFOOD_H
