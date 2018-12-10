//
// Created by tatiana on 13.06.2018.
//

#ifndef ZERL_DRINKITEM_H
#define ZERL_DRINKITEM_H

#include "game/behavior/Sequence.h"

namespace behavior {
    class DrinkItem : public Sequence{
    public:
        DrinkItem();
        ~DrinkItem();

        static TaskResult drink(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERL_DRINKITEM_H
