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
        addNode(std::shared_ptr<Condition>(new Condition(&SatisfyStarvation::starving)));
        addNode(std::shared_ptr<StopStarvationOrEatAtTable>(new StopStarvationOrEatAtTable));
    }

    SatisfyStarvation::~SatisfyStarvation()
    {

    }

    TaskResult SatisfyStarvation::starving(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return ((character->isStarving()) || (character->isLookingForFood())) ? TaskResult::Success : TaskResult::Failure;
    }
}