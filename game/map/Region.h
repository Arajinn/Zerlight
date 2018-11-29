//
// Created by tatiana.polozova on 27.03.2018.
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
    private:
        std::shared_ptr<Map> mMap;
        std::shared_ptr<game::Fortress> mFortress;
    };
}

#endif //ZERL_REGION_H
