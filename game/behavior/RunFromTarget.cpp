//
// Created by tatiana on 24.12.18.
//

#include "RunFromTarget.h"
#include "Condition.h"
#include "Action.h"
#include "game/core/Character.h"

namespace behavior {
    RunFromTarget::RunFromTarget()
            :Sequence()
    {
        addNode(std::make_shared<Condition>(&RunFromTarget::shouldRunFromTarget));
        addNode(std::make_shared<Action>(&RunFromTarget::runAway));
    }

    RunFromTarget::~RunFromTarget()
    {

    }

    TaskResult RunFromTarget::shouldRunFromTarget(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return ((character->shouldRunFromTarget()) && (!character->shouldStopRunning())) ? TaskResult::Success : TaskResult::Failure;
    }

    TaskResult RunFromTarget::runAway(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return character->runAwayFromTarget() ? TaskResult::Success : TaskResult::Failure;
    }
}