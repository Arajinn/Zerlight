//
// Created by tatiana.polozova on 22.06.2018.
//

#include "PathToFood.h"
#include "Condition.h"
#include "Action.h"
#include "game/core/Character.h"
#include "game/core/Pathfinder.h"

namespace behavior
{
    PathToFood::PathToFood()
        :Selector()
    {
        addNode(std::shared_ptr<Condition>(new Condition(&PathToFood::pathingToFood)));
        addNode(std::shared_ptr<Action>(new Action(&PathToFood::pathTo)));
    }

    PathToFood::~PathToFood()
    {

    }

    TaskResult PathToFood::pathTo(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        auto ent=character->currentNeedGoal();
        if (ent==nullptr)
            return TaskResult::Failure;

        return character->pathTo(ent->position(),false) ? TaskResult::Running : TaskResult::Failure;
    }

    TaskResult PathToFood::pathingToFood(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        if (character->pathfinder()->pathing())
            return (character->currentNeedGoal()->position()==character->desiredPosition()) ? TaskResult::Running : TaskResult::Failure;

        return ((character->atEndPosition()) && (character->currentNeedGoal()->position()==character->desiredPosition())) ? TaskResult::Success : TaskResult::Failure;
    }
}