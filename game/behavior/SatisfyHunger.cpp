//
// Created by tatiana on 26.06.2018.
//

#include "SatisfyHunger.h"
#include "Condition.h"
#include "GetFedOrFeedSelfAtTable.h"
#include "game/core/Character.h"

namespace behavior
{
    SatisfyHunger::SatisfyHunger()
            :Sequence()
    {
        addNode(std::shared_ptr<Condition>(new Condition(&SatisfyHunger::hungry)));
        addNode(std::shared_ptr<GetFedOrFeedSelfAtTable>(new GetFedOrFeedSelfAtTable));
    }

    SatisfyHunger::~SatisfyHunger()
    {

    }

    TaskResult SatisfyHunger::hungry(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return character->isHungry() ? TaskResult::Success : TaskResult::Failure;
    }
}