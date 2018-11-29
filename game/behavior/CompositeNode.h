//
// Created by tatiana.polozova on 26.03.2018.
//

#ifndef ZERL_COMPOSITE_NODE_H
#define ZERL_COMPOSITE_NODE_H

#include "game/behavior/Node.h"

#include <vector>
#include <memory>

namespace behavior
{
    class CompositeNode : public Node
    {
    public:
        CompositeNode();
        ~CompositeNode();

        void addNode(std::shared_ptr<Node> child);

    protected:
        std::vector<std::shared_ptr<Node>> mChildren;
    };
}

#endif //ZERL_COMPOSITE_NODE_H
