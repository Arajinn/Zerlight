//
// Created by tatiana.polozova on 27.03.2018.
//

#ifndef ZERL_MAP_H
#define ZERL_MAP_H

#include "game/map/vector3.h"

#include <vector>
#include <memory>

namespace map
{
    class MapCell;
    class NavGraph;
}

namespace map {
    class Map {
    public:
        Map();
        ~Map();

        void testMap();
        void postInit();

        const std::shared_ptr<MapCell> cell(const vector3& pos) const;
        const std::shared_ptr<MapCell> cell(const int& x,const int& y,const int& z) const;
        bool isWalkable(const vector3& pos) const;
        bool inBounds(const vector3& pos) const;
        bool inBounds(int x, int y, int z) const;

        const int& mapWidth() const;
        const int& mapHeight() const;
        const int& mapDepth() const;

        const std::shared_ptr<NavGraph> navGraph();

        static float distanceSquaredWithBias(const vector3& pos1, const vector3& pos2);
    private:
        std::vector<std::vector<std::vector<std::shared_ptr<MapCell>>>> mMap;
        int mMapWidth, mMapHeight, mMapDepth;

        std::shared_ptr<NavGraph> mNavGraph;
    };
}

#endif //ZERL_MAP_H
