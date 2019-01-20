//
// Created by tatiana on 23.12.18.
//

#ifndef ZERLIGHT_PICKUPWEAPON_H
#define ZERLIGHT_PICKUPWEAPON_H

#include "game/behavior/Sequence.h"

namespace behavior {
    class PickupWeapon : public Sequence {
    public:
        PickupWeapon();
        ~PickupWeapon();

        static TaskResult haveTarget(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult canPickupWeapon(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
        static TaskResult pickupRandomWeapon(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node);
    };
}

#endif //ZERLIGHT_PICKUPWEAPON_H
