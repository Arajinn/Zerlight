//
// Created by tatiana.polozova on 26.06.2018.
//

#ifndef ZERL_SATISFYSTARVATION_H
#define ZERL_SATISFYSTARVATION_H

#include "game/behavior/Sequence.h"

namespace behavior {
    class SatisfyStarvation : public Sequence{
    public:
        SatisfyStarvation();
        ~SatisfyStarvation();

        static TaskResult starving(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERL_SATISFYSTARVATION_H
