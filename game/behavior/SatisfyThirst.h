//
// Created by tatiana on 08.06.2018.
//

#ifndef ZERL_SATISFYTHIRST_H
#define ZERL_SATISFYTHIRST_H

#include "game/behavior/Sequence.h"

namespace behavior {
    class SatisfyThirst : public Sequence {
    public:
        SatisfyThirst();
        ~SatisfyThirst();

        static TaskResult thirsty(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERL_SATISFYTHIRST_H
