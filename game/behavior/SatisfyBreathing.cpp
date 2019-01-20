//
// Created by tatiana on 18.12.18.
//

#include "SatisfyBreathing.h"
#include "Condition.h"
#include "game/core/Character.h"
#include "game/behavior/PathToDryLand.h"

namespace behavior
{
    SatisfyBreathing::SatisfyBreathing()
            :Sequence()
    {
        addNode(std::make_shared<Condition>(&SatisfyBreathing::isSuffocating));
        addNode(std::make_shared<Condition>(&SatisfyBreathing::doesNotHaveJob));
        addNode(std::make_shared<PathToDryLand>());
    }

    SatisfyBreathing::~SatisfyBreathing()
    {

    }

    TaskResult SatisfyBreathing::isSuffocating(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return character->isSuffocating() ? TaskResult::Success : TaskResult::Failure;
    }

    TaskResult SatisfyBreathing::doesNotHaveJob(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return (character->job()== nullptr) ? TaskResult::Success : TaskResult::Failure;
    }
}