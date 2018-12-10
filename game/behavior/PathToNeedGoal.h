//
// Created by tatiana on 6/15/2018.
//

#ifndef ZERL_PATHTONEEDGOAL_H
#define ZERL_PATHTONEEDGOAL_H

#include "game/behavior/Selector.h"

namespace behavior {
    class PathToNeedGoal : public Selector{
    public:
        PathToNeedGoal();
        ~PathToNeedGoal();

        static TaskResult pathTo(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult pathingToNeed(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERL_PATHTONEEDGOAL_H
