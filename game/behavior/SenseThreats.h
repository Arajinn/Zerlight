//
// Created by tatiana on 06.01.19.
//

#ifndef ZERLIGHT_SENSETHREATS_H
#define ZERLIGHT_SENSETHREATS_H

#include "game/behavior/Selector.h"

namespace behavior {
    class SenseThreats : public Selector {
    public:
        SenseThreats();
        ~SenseThreats();

        static TaskResult dropExtraWeapons(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERLIGHT_SENSETHREATS_H
