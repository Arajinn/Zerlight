//
// Created by tatiana on 08.06.2018.
//

#include "WanderInGeneral.h"
#include "Condition.h"
#include "GeneralWander.h"
#include "game/core/Character.h"

namespace behavior
{
    WanderInGeneral::WanderInGeneral()
            :Sequence()
    {
        addNode(std::make_shared<Condition>(&WanderInGeneral::notClaimed));
        addNode(std::make_shared<GeneralWander>());
    }

    WanderInGeneral::~WanderInGeneral()
    {

    }

    TaskResult WanderInGeneral::notClaimed(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return !character->claimed() ? TaskResult::Success : TaskResult::Failure;
    }
}