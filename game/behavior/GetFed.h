//
// Created by tatiana.polozova on 6/15/2018.
//

#ifndef ZERL_GETFED_H
#define ZERL_GETFED_H

#include "game/behavior/Sequence.h"

namespace behavior {
    class GetFed : public Sequence{
    public:
        GetFed();
        ~GetFed();

        static TaskResult noGrip(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERL_GETFED_H
