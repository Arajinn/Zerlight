//
// Created by tatiana on 24.12.18.
//

#ifndef ZERLIGHT_PATHTOENEMY_H
#define ZERLIGHT_PATHTOENEMY_H

#include "game/behavior/Selector.h"

namespace behavior {
    class PathToEnemy : public Selector {
    public:
        PathToEnemy();
        ~PathToEnemy();

        static TaskResult withinRange(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult pathTo(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult pathingToEnemy(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERLIGHT_PATHTOENEMY_H
