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
        addNode(std::make_shared<Condition>(&SatisfyHunger::hungry));
        addNode(std::make_shared<GetFedOrFeedSelfAtTable>());
    }

    SatisfyHunger::~SatisfyHunger()
    {

    }

    TaskResult SatisfyHunger::hungry(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return character->isHungry() ? TaskResult::Success : TaskResult::Failure;
    }
}