//
// Created by tatiana on 06.01.19.
//

#ifndef ZERLIGHT_ENGAGETARGET_H
#define ZERLIGHT_ENGAGETARGET_H

#include "game/behavior/Sequence.h"

namespace behavior {
    class EngageTarget : public Sequence{
    public:
        EngageTarget();
        ~EngageTarget();

        static TaskResult haveTarget(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult attackTarget(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERLIGHT_ENGAGETARGET_H
