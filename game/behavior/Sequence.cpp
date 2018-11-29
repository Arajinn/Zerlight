//
// Created by tatiana.polozova on 08.06.2018.
//

#include "Sequence.h"

namespace behavior
{
    Sequence::Sequence()
            :CompositeNode()
    {

    }

    Sequence::~Sequence()
    {

    }

    TaskResult Sequence::execute(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        for (auto child : mChildren)
        {
            TaskResult taskResult=child->execute(character,dt,child);
            if (taskResult!=TaskResult::Success)
                return taskResult;
        }

        return TaskResult::Success;
    }
}