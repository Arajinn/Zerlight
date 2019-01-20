//
// Created by tatiana on 08.06.2018.
//

#ifndef ZERL_SEQUENCE_H
#define ZERL_SEQUENCE_H

#include "game/behavior/CompositeNode.h"

namespace behavior {
    class Sequence : public CompositeNode {
    public:
        Sequence();
        virtual ~Sequence();

        TaskResult execute(std::shared_ptr<game::Character>, float dt, std::shared_ptr<Node> node=nullptr) override;
    };
}

#endif //ZERL_SEQUENCE_H
