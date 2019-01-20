//
// Created by tatiana on 18.12.18.
//

#include "PathToDryLand.h"
#include "Condition.h"
#include "game/core/Character.h"
#include "game/core/Pathfinder.h"
#include "game/core/defines.h"
#include "game/map/MapCell.h"

namespace behavior
{
    PathToDryLand::PathToDryLand()
            :Sequence()
    {
        addNode(std::make_shared<Condition>(&PathToDryLand::pathingToDryLand));
        addNode(std::make_shared<Condition>(&PathToDryLand::pathTo));
    }

    PathToDryLand::~PathToDryLand()
    {

    }

    TaskResult PathToDryLand::pathingToDryLand(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return character->findDryLand() ? TaskResult::Running : TaskResult::Failure;
    }

    TaskResult PathToDryLand::pathTo(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        if (!character->pathfinder()->pathing())
            return TaskResult::Failure;

        const auto cell=WORLD_MAP->cell(character->desiredPosition());
        return ((!cell->willSuffocate()) && (!cell->hasLava())) ? TaskResult::Running : TaskResult::Failure;
    }
}