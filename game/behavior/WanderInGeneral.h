//
// Created by tatiana on 08.06.2018.
//

#ifndef ZERL_WANDERINGENERAL_H
#define ZERL_WANDERINGENERAL_H

#include "game/behavior/Sequence.h"

namespace behavior {
    class WanderInGeneral : public Sequence {
    public:
        WanderInGeneral();
        ~WanderInGeneral();

        static TaskResult notClaimed(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERL_WANDERINGENERAL_H
