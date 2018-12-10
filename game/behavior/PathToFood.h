//
// Created by tatiana on 22.06.2018.
//

#ifndef ZERL_PATHTOFOOD_H
#define ZERL_PATHTOFOOD_H

#include "game/behavior/Selector.h"

namespace behavior {
    class PathToFood : public Selector {
    public:
        PathToFood();
        ~PathToFood();

        static TaskResult pathTo(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult pathingToFood(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERL_PATHTOFOOD_H
