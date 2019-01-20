//
// Created by tatiana on 28.03.2018.
//

#ifndef ZERL_NAVGRAPHNODE_H
#define ZERL_NAVGRAPHNODE_H

#include "game/map/vector3.h"
#include "game/map/rectangle2.h"

#include <vector>
#include <memory>

namespace map {
class NavGraphNode : public std::enable_shared_from_this<NavGraphNode> {
    public:
        NavGraphNode(int id);
        ~NavGraphNode();

        void setArea(rectangle2 area, int level);

        const rectangle2& area() const;
        const int& level() const;

        const int& nodeNavID() const;
        //void setNodeNavID(int id);

        const int& navGraphID() const;
        void setNavGraphID(int id);

        static void makeConnection(std::shared_ptr<NavGraphNode> node1,std::shared_ptr<NavGraphNode> node2);

        void clearConnections();

        size_t connectionsCount() const;
        std::vector<unsigned int> connectionsID();

        std::vector<std::shared_ptr<NavGraphNode>> connections() const;

        bool equals(std::shared_ptr<NavGraphNode> node) const;

        bool contains(const map::vector3& position);
        bool contains2D(const map::vector3& position) const;

        map::vector3 closestPoint(const map::vector3& position) const;

        bool hasDryLand() const;
        bool hasDryLand(map::vector3& position) const;
    private:
        int mNodeID;
        int mNavGraphID;

        rectangle2 mArea;
        vector3 mCenter;
        int mLevel;

        void findCenter();

        std::vector<std::shared_ptr<NavGraphNode>> mConnections;
        void addConnection(std::shared_ptr<NavGraphNode> node);

        void removeConnection(std::shared_ptr<NavGraphNode> node);
    };
}

#endif //ZERL_NAVGRAPHNODE_H
