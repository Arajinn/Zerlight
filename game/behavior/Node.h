//
// Created by tatiana on 08.06.2018.
//

#ifndef ZERL_NODE_H
#define ZERL_NODE_H

#include "game/behavior/TaskResult.h"

#include <memory>

namespace game
{
    class Character;
}

namespace behavior {
    class Node;

    typedef TaskResult (*task_ptr)(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);

    class Node {
    public:
        Node();
        virtual ~Node();

        virtual TaskResult execute(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node=nullptr) = 0;

    public:
        float time;
    };
}

#endif //ZERL_NODE_H
