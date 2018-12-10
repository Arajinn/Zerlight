//
// Created by tatiana on 2018-04-18.
//

#ifndef ZERL_NAVGRAPH_H
#define ZERL_NAVGRAPH_H

#include "game/map/rectangle2.h"

#include <memory>
#include <vector>

namespace map
{
    class NavGraphNode;
    class MapCell;
}

namespace map {
    class NavGraph {
    public:
        NavGraph();
        ~NavGraph();

        void removeTile(std::shared_ptr<MapCell> mapCell);
        void addTile(std::shared_ptr<MapCell> mapCell);
        void reestablishConnections(std::shared_ptr<MapCell> mapCell);
    private:
        void subdivideIntoAllowedAreas(map::rectangle2 bounds, int levelStart, int levelEnd);
        std::shared_ptr<NavGraphNode> growSquare(rectangle2 bounds, int level);
        void createConnections(std::shared_ptr<NavGraphNode> node);
        void mergeNavGraphIDs(std::shared_ptr<NavGraphNode> node);
        bool mergeSquareNodeWithNeighbours(std::shared_ptr<NavGraphNode> node);
        void propagateNavGraphID(std::shared_ptr<NavGraphNode> node, unsigned int id);

        void setNewNodeOnCells(const rectangle2& area, const int& level, std::shared_ptr<NavGraphNode> node);
        void setNewNodeOnCells(std::shared_ptr<NavGraphNode> oldNode, std::shared_ptr<NavGraphNode> newNode);

        std::shared_ptr<NavGraphNode> largestNeighbour(std::shared_ptr<NavGraphNode> node);
        bool growLeftRightTopBottom(int& left, int& right, int& top, int& bottom, int& level, std::vector<std::shared_ptr<NavGraphNode>>& victimNodes);
        bool growRight(const int& left, const int& right, const int& top, const int& bottom, const int& level, std::vector<std::shared_ptr<NavGraphNode>>& victimNodes);
        bool growLeft(const int& left, const int& right, const int& top, const int& bottom, const int& level, std::vector<std::shared_ptr<NavGraphNode>>& victimNodes);
        bool growTopBottom(const int& left, const int& right, int& top, int& bottom, const int& level, std::vector<std::shared_ptr<NavGraphNode>>& victimNodes);
        bool growTop(const int& left, const int& right, const int& top, const int& bottom, const int& level, std::vector<std::shared_ptr<NavGraphNode>>& victimNodes);
        bool growBottom(const int& left, const int& right, const int& top, const int& bottom, const int& level, std::vector<std::shared_ptr<NavGraphNode>>& victimNodes);
    };
}

#endif //ZERL_NAVGRAPH_H
