//
// Created by tatiana on 06.01.19.
//

#include "EngageTarget.h"
#include "Condition.h"
#include "GetInPosition.h"
#include "game/core/Character.h"
#include "game/core/Pathfinder.h"

namespace behavior {
    EngageTarget::EngageTarget()
            :Sequence()
    {
        addNode(std::make_shared<Condition>(&EngageTarget::haveTarget));
        addNode(std::make_shared<GetInPosition>());
        addNode(std::make_shared<Condition>(&EngageTarget::attackTarget));
    }

    EngageTarget::~EngageTarget()
    {

    }

    TaskResult EngageTarget::haveTarget(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return (character->currentTarget()!=nullptr) ? TaskResult::Success : TaskResult::Failure;
    }

    TaskResult EngageTarget::attackTarget(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        if (!character->pathfinder()->pathing())
            character->attackTarget(dt);

        return TaskResult::Success;
    }
}