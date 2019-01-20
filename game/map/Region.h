//
// Created by tatiana on 27.03.2018.
//

#ifndef ZERL_REGION_H
#define ZERL_REGION_H

#include <memory>

namespace game
{
    class Fortress;
}

namespace map
{
    class Map;
}

namespace map {
    class Region {
    public:
        Region();
        ~Region();

        const std::shared_ptr<Map> map() const;
        const std::shared_ptr<game::Fortress> fortress() const;

        void processSpawn();
        void processDeletion();

        float insideLight() const;
        float outsideLight() const;

        float time() const;
        unsigned int day() const;
        float totalTime() const;
    private:
        std::shared_ptr<Map> mMap;
        std::shared_ptr<game::Fortress> mFortress;

        float mInsideLight,mOutsideLight;

        float mAxis,mLatitude,mDayCalcConstant;
        unsigned int mDay;
        float mDayLength;
        float calculateDayLength(const int& rawDay) const;
        float mTime;
        float sunrise() const;
    };
}

#endif //ZERL_REGION_H
