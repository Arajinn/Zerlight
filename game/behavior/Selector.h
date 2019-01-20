//
// Created by tatiana on 08.06.2018.
//

#ifndef ZERL_SELECTOR_H
#define ZERL_SELECTOR_H

#include "game/behavior/CompositeNode.h"

namespace behavior
{
    class Selector : public CompositeNode {
    public:
        Selector();
        virtual ~Selector();

        TaskResult execute(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node=nullptr) override;
    };
}

#endif //ZERL_SELECTOR_H
