//
// Created by tatiana on 25.06.2018.
//

#include "PathToChair.h"
#include "Condition.h"
#include "Action.h"
#include "game/core/Character.h"
#include "game/core/Pathfinder.h"

namespace behavior
{
    PathToChair::PathToChair()
            :Selector()
    {
        addNode(std::make_shared<Condition>(&PathToChair::pathingToChair));
        addNode(std::make_shared<Action>(&PathToChair::pathTo));
    }

    PathToChair::~PathToChair()
    {

    }

    TaskResult PathToChair::pathTo(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        if (character->currentNeedGoal()==nullptr)
            return TaskResult::Failure;

        return (character->pathTo(character->currentNeedGoal()->position(),false)) ? TaskResult::Failure : TaskResult::Running;
    }

    TaskResult PathToChair::pathingToChair(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        if (character->currentNeedGoal()==nullptr)
            return TaskResult::Failure;

        if (character->pathfinder()->pathing())
            return (character->currentNeedGoal()->position()==character->desiredPosition()) ? TaskResult::Running : TaskResult::Failure;

        return ((character->atEndPosition()) && (character->currentNeedGoal()->position()==character->desiredPosition())) ? TaskResult::Success : TaskResult::Failure;
    }
}