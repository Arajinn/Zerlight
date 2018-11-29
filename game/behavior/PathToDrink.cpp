//
// Created by tatiana.polozova on 13.06.2018.
//

#include "PathToDrink.h"
#include "Condition.h"
#include "Action.h"
#include "game/core/Character.h"
#include "game/core/Construction.h"
#include "game/core/Pathfinder.h"

namespace behavior
{
    PathToDrink::PathToDrink()
            :Selector()
    {
        addNode(std::shared_ptr<Condition>(new Condition(&PathToDrink::pathingToDrink)));
        addNode(std::shared_ptr<Action>(new Action(&PathToDrink::pathTo)));
    }

    PathToDrink::~PathToDrink()
    {

    }

    TaskResult PathToDrink::pathTo(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        bool adjacent=false;

        auto ent=character->currentNeedGoal();
        if (ent==nullptr)
            return TaskResult::Failure;

        auto castToConstruction=std::dynamic_pointer_cast<game::Construction>(ent);
        if (castToConstruction!= nullptr)
        {

        }

        return character->pathTo(ent->position(),adjacent) ? TaskResult::Running : TaskResult::Failure;
    }

    TaskResult PathToDrink::pathingToDrink(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        if (character->pathfinder()->pathing())
            return (character->currentNeedGoal()->position()==character->desiredPosition()) ? TaskResult::Running : TaskResult::Failure;

        return ((character->atEndPosition()) && (character->currentNeedGoal()->position()==character->desiredPosition())) ? TaskResult::Success : TaskResult::Failure;
    }
}