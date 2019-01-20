//
// Created by tatiana on 26.06.2018.
//

#include "SatisfyStarvation.h"
#include "Condition.h"
#include "StopStarvationOrEatAtTable.h"
#include "game/core/Character.h"

namespace behavior
{
    SatisfyStarvation::SatisfyStarvation()
            :Sequence()
    {
        addNode(std::make_shared<Condition>(&SatisfyStarvation::starving));
        addNode(std::make_shared<StopStarvationOrEatAtTable>());
    }

    SatisfyStarvation::~SatisfyStarvation()
    {

    }

    TaskResult SatisfyStarvation::starving(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return ((character->isStarving()) || (character->isLookingForFood())) ? TaskResult::Success : TaskResult::Failure;
    }
}