//
// Created by tatiana on 25.06.2018.
//

#ifndef ZERL_LOOKFORCHAIR_H
#define ZERL_LOOKFORCHAIR_H

#include "game/behavior/Selector.h"

namespace behavior {
    class LookForChair : public Selector{
    public:
        LookForChair();
        ~LookForChair();

        static TaskResult haveChair(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult findChair(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERL_LOOKFORCHAIR_H
