//
// Created by tatiana on 12.01.19.
//

#ifndef ZERLIGHT_MILITARYSTATION_H
#define ZERLIGHT_MILITARYSTATION_H

#include "game/core/Designation.h"

namespace game {
    class MilitaryStation : public Designation{
    public:
        MilitaryStation();
        virtual ~MilitaryStation();

        static int currentShift();
    };
}

#endif //ZERLIGHT_MILITARYSTATION_H
