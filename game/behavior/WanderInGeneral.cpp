//
// Created by tatiana.polozova on 08.06.2018.
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
        addNode(std::shared_ptr<Condition>(new Condition(&WanderInGeneral::notClaimed)));
        addNode(std::shared_ptr<GeneralWander>(new GeneralWander()));
    }

    WanderInGeneral::~WanderInGeneral()
    {

    }

    TaskResult WanderInGeneral::notClaimed(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return !character->claimed() ? TaskResult::Success : TaskResult::Failure;
    }
}