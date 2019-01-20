//
// Created by tatiana on 25.12.18.
//

#include "GetInPosition.h"
#include "Action.h"
#include "PathToEnemy.h"
#include "game/core/Character.h"

namespace behavior {
    GetInPosition::GetInPosition()
            :Selector()
    {
        addNode(std::make_shared<Action>(&GetInPosition::moveToMaxRange));
        addNode(std::make_shared<PathToEnemy>());
    }

    GetInPosition::~GetInPosition()
    {

    }

    TaskResult GetInPosition::moveToMaxRange(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        if (!character->shouldMoveToMaxRange())
            return TaskResult::Failure;

        character->moveToMaxRange();
        return TaskResult::Success;
    }
}