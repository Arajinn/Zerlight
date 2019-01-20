//
// Created by tatiana on 27.03.2018.
//

#include "Region.h"
#include "Map.h"
#include "game/core/Fortress.h"

#define _USE_MATH_DEFINES
#include <cmath>

namespace map
{
    Region::Region()
    {
        mMap=std::make_shared<Map>();
        mFortress=std::make_shared<game::Fortress>();

        mInsideLight=0.5f;
        mOutsideLight=0.75f;

        mAxis=23.439f/180.0f*float(M_PI);
        mLatitude=50.0f/180.0f*float(M_PI);
        mDayCalcConstant=float(M_PI)/24.0f;

        mDay=0;
        mDayLength=calculateDayLength(mDay);
        mTime=sunrise()-0.1f;
    }

    Region::~Region()
    {

    }

    const std::shared_ptr<Map> Region::map() const
    {
        return mMap;
    }

    const std::shared_ptr<game::Fortress> Region::fortress() const
    {
        return mFortress;
    }

    void Region::processSpawn()
    {
        mFortress->processSpawn();
    }

    void Region::processDeletion()
    {
        mFortress->processDeletion();
    }

    float Region::insideLight() const
    {
        return mInsideLight;
    }

    float Region::outsideLight() const
    {
        return mOutsideLight;
    }

    float Region::calculateDayLength(const int& rawDay) const
    {
        return std::acos(1.0f-(1.0f-std::tan(mLatitude)*std::tan(mAxis*std::cos(mDayCalcConstant*float(rawDay+6)))))/float(M_PI)*600.0f;
    }

    float Region::sunrise() const
    {
        return (600.0f-mDayLength)*0.5f;
    }

    float Region::time() const
    {
        return mTime;
    }

    unsigned int Region::day() const
    {
        return mDay;
    }

    float Region::totalTime() const
    {
        return float(mDay)+mTime/600.0f;
    }
}