//
// Created by tatiana on 25.12.18.
//

#ifndef ZERLIGHT_GETINPOSITION_H
#define ZERLIGHT_GETINPOSITION_H

#include "game/behavior/Selector.h"

namespace behavior {
    class GetInPosition : public Selector {
    public:
        GetInPosition();
        ~GetInPosition();

        static TaskResult moveToMaxRange(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERLIGHT_GETINPOSITION_H
