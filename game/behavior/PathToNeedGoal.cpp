//
// Created by tatiana on 6/15/2018.
//

#include "PathToNeedGoal.h"
#include "Condition.h"
#include "Action.h"
#include "game/core/Character.h"
#include "game/core/Pathfinder.h"

namespace behavior
{
    PathToNeedGoal::PathToNeedGoal()
            :Selector()
    {
        addNode(std::make_shared<Condition>(&PathToNeedGoal::pathingToNeed));
        addNode(std::make_shared<Action>(&PathToNeedGoal::pathTo));
    }

    PathToNeedGoal::~PathToNeedGoal()
    {

    }

    TaskResult PathToNeedGoal::pathTo(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        if (character->currentNeedGoal()==nullptr)
            return TaskResult::Success;

        return (character->pathTo(character->currentNeedGoal()->position(),false)) ? TaskResult::Running : TaskResult::Failure;
    }

    TaskResult PathToNeedGoal::pathingToNeed(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        if (character->currentNeedGoal()==nullptr)
            return TaskResult::Success;

        if (character->pathfinder()->pathing())
            return (character->currentNeedGoal()->position()==character->desiredPosition()) ? TaskResult::Running : TaskResult::Failure;

        return (character->atEndPosition() && (character->currentNeedGoal()->position()==character->desiredPosition())) ? TaskResult::Success : TaskResult::Failure;
    }
}