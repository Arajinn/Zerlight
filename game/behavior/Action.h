//
// Created by tatiana on 08.06.2018.
//

#ifndef ZERL_ACTION_H
#define ZERL_ACTION_H

#include "Node.h"

namespace game
{
    class Character;
}

namespace behavior {
    class Action : public Node {
    public:
        Action(task_ptr ptr, float start=0.0f);
        virtual ~Action();

        TaskResult execute(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node) override;

    private:
        task_ptr mAction;
    };
}

#endif //ZERL_ACTION_H
