//
// Created by tatiana on 06.01.19.
//

#ifndef ZERLIGHT_PATHTOLASTENEMYLOCATION_H
#define ZERLIGHT_PATHTOLASTENEMYLOCATION_H

#include "game/behavior/Selector.h"

namespace behavior {
    class PathToLastEnemyLocation : public Selector {
    public:
        PathToLastEnemyLocation();
        ~PathToLastEnemyLocation();

        static TaskResult pathingToLocation(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult pathTo(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERLIGHT_PATHTOLASTENEMYLOCATION_H
