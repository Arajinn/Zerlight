//
// Created by tatiana on 12.01.19.
//

#include "MilitaryStation.h"
#include "defines.h"
#include "game/map/Region.h"

namespace game
{
    MilitaryStation::MilitaryStation()
        :Designation()
    {

    }

    MilitaryStation::~MilitaryStation()
    {

    }

    int MilitaryStation::currentShift()
    {
        const float time=REGION->time()/600.0f*24.0f;

        if ((time>=6.0f) && (time<14.0f))
            return 0;
        else if ((time>=14.0f) && (time<22.0f))
            return 1;
        else
            return 2;
    }
}