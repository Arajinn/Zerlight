//
// Created by tatiana on 26.03.2018.
//

#ifndef ZERL_PATHFINDER_H
#define ZERL_PATHFINDER_H

#include "game/map/vector3.h"

#include <vector>
#include <memory>

namespace game
{
    class Character;
    class PathfinderNode;
    class NavGraphPathNode;
}

namespace map
{
    class NavGraphNode;
}

namespace game {
    class Pathfinder {
    public:
        Pathfinder(std::shared_ptr<Character> parent);
        ~Pathfinder();

        bool pathing() const;
        const map::vector3& nextPosition() const;
        map::vector3 endPosition() const;
        bool atEndPosition(const map::vector3& pos) const;

        void clearPath();

        void startNewPathing(const map::vector3& end_pos);

        bool stepPath();

        bool findNavPath(const map::vector3& start, const map::vector3& end, bool adjacent);

        bool canReach(const map::vector3& start, const map::vector3& end, bool adjacent);

        static bool isAdjacent(std::shared_ptr<map::NavGraphNode> node, const map::vector3& pos);
        static bool isAdjacent(const map::vector3& pos1, const map::vector3& pos2);
    private:
        std::shared_ptr<Character> owner;

        bool mPathing;
        bool mAdjacentPos;
        map::vector3 mEndPos;

        std::vector<map::vector3> vectorArrayFindPath;

        std::vector<map::vector3> mPath;
        std::vector<std::shared_ptr<map::NavGraphNode>> mNavPath;

        static bool isAdjacentToNavGraph(const map::vector3& pos, unsigned int startGraphID);
        static bool isAdjacent2D(const map::vector3& pos1, const map::vector3& pos2);
        static bool containsAdjacentSpot(std::shared_ptr<map::NavGraphNode> node, const map::vector3& pos);
        static bool isDirectlyAboveOrBelow(const map::vector3& pos1, const map::vector3& pos2);
        static bool isDiagonal(const map::vector3& pos1, const map::vector3& pos2);

        bool has2DLOS(map::vector3 start, map::vector3 end, std::vector<map::vector3> &path);

        bool findPath(const map::vector3& start, const map::vector3& end);

        void createPath(std::shared_ptr<PathfinderNode> end);
        void createNavPath(std::shared_ptr<NavGraphPathNode> node);
    };
}

#endif //ZERL_PATHFINDER_H
