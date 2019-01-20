//
// Created by tatiana on 27.12.18.
//

#ifndef ZERLIGHT_STOCKITEMJOB_H
#define ZERLIGHT_STOCKITEMJOB_H

#include "game/core/Job.h"

namespace game {
    class StockItemJob : public Job{
    public:
        StockItemJob(const map::vector3& position);
        ~StockItemJob();

    };
}

#endif //ZERLIGHT_STOCKITEMJOB_H
