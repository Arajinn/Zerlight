//
// Created by tatiana on 28.03.2018.
//

#include "PriorityQueue.h"
#include "game/map/NavGraphNode.h"

namespace game
{
    PriorityQueuePathfinderNode::PriorityQueuePathfinderNode()
    {

    }

    PriorityQueuePathfinderNode::PriorityQueuePathfinderNode(const float& a_priority, std::shared_ptr<PathfinderNode> a_node)
    {
        priority=a_priority;
        node=a_node;
    }

    PriorityQueuePathfinderNode::~PriorityQueuePathfinderNode()
    {

    }

    PathfinderNode::PathfinderNode()
    {

    }

    PathfinderNode::PathfinderNode(map::vector3 a_position)
        :position(a_position)
    {

    }

    bool PathfinderNode::equals(const map::vector3& a_position)
    {
        return (position==a_position);
    }

    PriorityQueueNavGraphPathNode::PriorityQueueNavGraphPathNode()
    {

    }

    PriorityQueueNavGraphPathNode::PriorityQueueNavGraphPathNode(const float& a_priority, std::shared_ptr<NavGraphPathNode> a_node)
    {
        priority=a_priority;
        node=a_node;
    }

    PriorityQueueNavGraphPathNode::~PriorityQueueNavGraphPathNode()
    {

    }

    NavGraphPathNode::NavGraphPathNode(std::shared_ptr<map::NavGraphNode> aNode)
            :node(aNode)
    {

    }

    bool NavGraphPathNode::equals(std::shared_ptr<NavGraphPathNode> a_node)
    {
        if (a_node==nullptr)
            return false;

        return a_node->node->equals(node);
    }
}