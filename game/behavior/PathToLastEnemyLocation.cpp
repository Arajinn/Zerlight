//
// Created by tatiana on 06.01.19.
//

#include "PathToLastEnemyLocation.h"
#include "Action.h"
#include "Condition.h"
#include "game/core/Character.h"
#include "game/core/Pathfinder.h"

namespace behavior {
    PathToLastEnemyLocation::PathToLastEnemyLocation()
            :Selector()
    {
        addNode(std::make_shared<Condition>(&PathToLastEnemyLocation::pathingToLocation));
        addNode(std::make_shared<Action>(&PathToLastEnemyLocation::pathTo));
    }

    PathToLastEnemyLocation::~PathToLastEnemyLocation()
    {

    }

    TaskResult PathToLastEnemyLocation::pathingToLocation(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        if (character->pathTo(character->lastEnemyLocation(),false))
            return TaskResult::Running;

        character->setTrackEnemy(false);
        return TaskResult::Failure;
    }

    TaskResult PathToLastEnemyLocation::pathTo(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        if (character->pathfinder()->pathing())
            return (character->lastEnemyLocation()==character->desiredPosition()) ? TaskResult::Running : TaskResult::Failure;

        if ((!character->atEndPosition()) || (character->lastEnemyLocation()!=character->desiredPosition()))
            return TaskResult::Failure;

        character->setTrackEnemy(false);
        return TaskResult::Success;
    }
}