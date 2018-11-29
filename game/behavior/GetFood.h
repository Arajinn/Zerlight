//
// Created by tatiana.polozova on 25.06.2018.
//

#ifndef ZERL_GETFOOD_H
#define ZERL_GETFOOD_H

#include "game/behavior/Selector.h"

namespace behavior {
    class GetFood : public Selector{
    public:
        GetFood();
        ~GetFood();

        static TaskResult haveFood(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERL_GETFOOD_H
