//
// Created by tatiana.polozova on 08.06.2018.
//

#ifndef ZERL_LOOKFORDRINK_H
#define ZERL_LOOKFORDRINK_H

#include "game/behavior/Selector.h"

namespace behavior {
    class LookForDrink : public Selector {
    public:
        LookForDrink();
        ~LookForDrink();

        static TaskResult haveDrink(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult findDrink(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERL_LOOKFORDRINK_H
