//
// Created by tatiana on 22.06.2018.
//

#ifndef ZERL_FREEHANDSFORITEM_H
#define ZERL_FREEHANDSFORITEM_H

#include "game/behavior/Selector.h"

namespace behavior {
    class FreeHandsForItem : public Selector {
    public:
        FreeHandsForItem();
        ~FreeHandsForItem();

        static TaskResult canCarry(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult sheathExtraItems(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult dropExraItems(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERL_FREEHANDSFORITEM_H
