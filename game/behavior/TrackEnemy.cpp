//
// Created by tatiana on 06.01.19.
//

#include "TrackEnemy.h"
#include "Condition.h"
#include "PathToLastEnemyLocation.h"
#include "game/core/Character.h"

namespace behavior {
    TrackEnemy::TrackEnemy()
            :Sequence()
    {
        addNode(std::make_shared<Condition>(&TrackEnemy::canTrackEnemy));
        addNode(std::make_shared<PathToLastEnemyLocation>());
    }

    TrackEnemy::~TrackEnemy()
    {

    }

    TaskResult TrackEnemy::canTrackEnemy(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return (character->trackEnemy()) ? TaskResult::Success : TaskResult::Failure;
    }
}