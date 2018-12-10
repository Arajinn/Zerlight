//
// Created by tatiana on 08.06.2018.
//

#include "Selector.h"

namespace behavior
{
    Selector::Selector()
            :CompositeNode()
    {

    }

    Selector::~Selector()
    {

    }

    TaskResult Selector::execute(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        for (auto child : mChildren)
        {
            TaskResult taskResult=child->execute(character,dt,child);
            if (taskResult!=TaskResult::Failure)
                return taskResult;
        }

        return TaskResult::Failure;
    }
}