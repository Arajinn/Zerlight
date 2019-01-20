//
// Created by tatiana on 18.12.18.
//

#ifndef ZERLIGHT_PATHTODRYLAND_H
#define ZERLIGHT_PATHTODRYLAND_H

#include "game/behavior/Sequence.h"

namespace behavior {
    class PathToDryLand : public Sequence{
    public:
        PathToDryLand();
        ~PathToDryLand();

        static TaskResult pathingToDryLand(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult pathTo(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERLIGHT_PATHTODRYLAND_H
