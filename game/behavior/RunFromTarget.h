//
// Created by tatiana on 24.12.18.
//

#ifndef ZERLIGHT_RUNFROMTARGET_H
#define ZERLIGHT_RUNFROMTARGET_H

#include "game/behavior/Sequence.h"

namespace behavior {
    class RunFromTarget : public Sequence {
    public:
        RunFromTarget();
        ~RunFromTarget();

        static TaskResult shouldRunFromTarget(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult runAway(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERLIGHT_RUNFROMTARGET_H
