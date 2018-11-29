//
// Created by tatiana.polozova on 22.06.2018.
//

#ifndef ZERL_LOOKFORFOOD_H
#define ZERL_LOOKFORFOOD_H

#include "game/behavior/Selector.h"

namespace behavior {
    class LookForFood : public Selector {
    public:
        LookForFood();
        ~LookForFood();

        static TaskResult haveFood(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult findFood(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERL_LOOKFORFOOD_H
