//
// Created by tatiana on 27.03.2018.
//

#ifndef ZERL_MAP_H
#define ZERL_MAP_H

#include "game/map/vector3.h"

#include <vector>
#include <memory>

namespace game
{
    struct CreateWorldOptions;
}

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

        bool testMap();
        bool generateMap(std::shared_ptr<game::CreateWorldOptions> worldOptions);
        void postInit();

        const std::shared_ptr<MapCell> cell(const vector3& pos) const;
        const std::shared_ptr<MapCell> cell(const int& x,const int& y,const int& z) const;
        bool isWalkable(const vector3& pos) const;
        bool inBounds(const vector3& pos) const;
        bool inBounds(int x, int y, int z) const;

        const size_t& mapWidth() const;
        const size_t& mapHeight() const;
        const size_t& mapDepth() const;

        const std::shared_ptr<NavGraph> navGraph();

        static float distanceSquaredWithBias(const vector3& pos1, const vector3& pos2);

        float getInitProgress() const;
    private:
        std::vector<std::vector<std::vector<std::shared_ptr<MapCell>>>> mMap;
        size_t mMapWidth, mMapHeight, mMapDepth;

        std::shared_ptr<NavGraph> mNavGraph;

        float mInitProgress;

        void propagateVisibility(const int& x, const int& y, const int& z);

        void generateRamps();
    };
}

#endif //ZERL_MAP_H
