//
// Created by tatiana on 06.01.19.
//

#ifndef ZERLIGHT_TRACKENEMY_H
#define ZERLIGHT_TRACKENEMY_H

#include "game/behavior/Sequence.h"

namespace behavior {
    class TrackEnemy : public Sequence{
    public:
        TrackEnemy();
        ~TrackEnemy();

        static TaskResult canTrackEnemy(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERLIGHT_TRACKENEMY_H
