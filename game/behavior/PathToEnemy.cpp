//
// Created by tatiana on 24.12.18.
//

#include "PathToEnemy.h"
#include "Condition.h"
#include "Action.h"
#include "game/core/Character.h"
#include "game/core/Pathfinder.h"

namespace behavior {
    PathToEnemy::PathToEnemy()
            :Selector()
    {
        addNode(std::make_shared<Condition>(&PathToEnemy::withinRange));
        addNode(std::make_shared<Condition>(&PathToEnemy::pathingToEnemy));
        addNode(std::make_shared<Action>(&PathToEnemy::pathTo));
    }

    PathToEnemy::~PathToEnemy()
    {

    }

    TaskResult PathToEnemy::withinRange(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return (character->withinTargetRange()) ? TaskResult::Success : TaskResult::Failure;
    }

    TaskResult PathToEnemy::pathingToEnemy(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        if (character->pathfinder()->pathing())
            return (character->currentTarget()->position()==character->desiredPosition()) ? TaskResult::Running : TaskResult::Failure;

        return ((character->atEndPosition()) && (character->currentTarget()->position()==character->desiredPosition())) ? TaskResult::Success : TaskResult::Failure;
    }

    TaskResult PathToEnemy::pathTo(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return (character->pathTo(character->currentTarget()->position(),true)) ? TaskResult::Running : TaskResult::Failure;
    }
}