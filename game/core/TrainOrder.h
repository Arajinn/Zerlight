//
// Created by tatiana on 27.12.18.
//

#ifndef ZERLIGHT_TRAINORDER_H
#define ZERLIGHT_TRAINORDER_H

#include "game/core/Job.h"

namespace game {
    class TrainOrder : public Job{
    public:
        TrainOrder(const map::vector3& position);
        ~TrainOrder();
    };
}

#endif //ZERLIGHT_TRAINORDER_H
