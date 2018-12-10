//
// Created by tatiana on 26.03.2018.
//

#include "CompositeNode.h"

namespace behavior
{
    CompositeNode::CompositeNode()
            :Node()
    {

    }

    CompositeNode::~CompositeNode()
    {

    }

    void CompositeNode::addNode(std::shared_ptr<Node> child)
    {
        mChildren.push_back(child);
    }
}