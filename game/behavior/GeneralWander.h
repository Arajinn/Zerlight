//
// Created by tatiana on 08.06.2018.
//

#ifndef ZERL_GENERALWANDER_H
#define ZERL_GENERALWANDER_H

#include "game/behavior/Selector.h"

namespace behavior {
    class GeneralWander : public Selector {
    public:
        GeneralWander();
        ~GeneralWander();

        static TaskResult pathing(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult wait(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult pathToNeighbourNode(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);

    private:
        float time;
    };
}

#endif //ZERL_GENERALWANDER_H
