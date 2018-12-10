//
// Created by tatiana on 25.06.2018.
//

#ifndef ZERL_PATHTOCHAIR_H
#define ZERL_PATHTOCHAIR_H

#include "game/behavior/Selector.h"

namespace behavior {
    class PathToChair : public Selector {
    public:
        PathToChair();
        ~PathToChair();

        static TaskResult pathTo(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult pathingToChair(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERL_PATHTOCHAIR_H
