//
// Created by tatiana on 08.06.2018.
//

#ifndef ZERL_CONDITION_H
#define ZERL_CONDITION_H

#include "Node.h"

namespace game
{
    class Character;
}

namespace behavior {
    class Condition : public Node {
    public:
        Condition(task_ptr ptr, float start=0.0f);
        virtual ~Condition();

        TaskResult execute(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node) override;
    private:
        task_ptr mCondition;
    };
}

#endif //ZERL_CONDITION_H
