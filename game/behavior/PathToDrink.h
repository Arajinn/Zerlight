//
// Created by tatiana on 13.06.2018.
//

#ifndef ZERL_PATHTODRINK_H
#define ZERL_PATHTODRINK_H

#include "game/behavior/Selector.h"

namespace behavior {
    class PathToDrink : public Selector{
    public:
        PathToDrink();
        ~PathToDrink();

        static TaskResult pathTo(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult pathingToDrink(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERL_PATHTODRINK_H
