//
// Created by tatiana on 28.03.2018.
//

#ifndef ZERL_PRIORITYQUEUE_H
#define ZERL_PRIORITYQUEUE_H

#include "game/map/vector3.h"

#include <memory>

namespace map
{
    class NavGraphNode;
}

namespace game {
    struct PathfinderNode
    {
        PathfinderNode();
        PathfinderNode(map::vector3 a_position);

        float distanceCost;
        float heuristicCost;

        map::vector3 position;
        map::vector3 closestPoint;

        std::shared_ptr<PathfinderNode> next;

        bool equals(const map::vector3& a_position);
    };

    struct PriorityQueuePathfinderNode
    {
        PriorityQueuePathfinderNode();
        PriorityQueuePathfinderNode(const float& a_priority, std::shared_ptr<PathfinderNode> a_node);
        ~PriorityQueuePathfinderNode();

        float priority;
        std::shared_ptr<PathfinderNode> node;
    };

    struct NavGraphPathNode
    {
        NavGraphPathNode();
        NavGraphPathNode(std::shared_ptr<map::NavGraphNode> aNode);

        float distanceCost;
        float heuristicCost;

        map::vector3 closestPoint;

        std::shared_ptr<map::NavGraphNode> node;
        std::shared_ptr<NavGraphPathNode> next;

        bool equals(std::shared_ptr<NavGraphPathNode> a_node);
    };

    struct PriorityQueueNavGraphPathNode {
        PriorityQueueNavGraphPathNode();
        PriorityQueueNavGraphPathNode(const float& a_priority, std::shared_ptr<NavGraphPathNode> a_node);
        ~PriorityQueueNavGraphPathNode();

        float priority;
        std::shared_ptr<NavGraphPathNode> node;
    };
}

#endif //ZERL_PRIORITYQUEUE_H
