//
// Created by tatiana on 18.12.18.
//

#ifndef ZERLIGHT_SATISFYBREATHING_H
#define ZERLIGHT_SATISFYBREATHING_H

#include "game/behavior/Sequence.h"

namespace behavior {
    class SatisfyBreathing : public Sequence{
    public:
        SatisfyBreathing();
        ~SatisfyBreathing();

        static TaskResult isSuffocating(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult doesNotHaveJob(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERLIGHT_SATISFYBREATHING_H
