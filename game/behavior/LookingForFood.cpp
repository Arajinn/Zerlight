//
// Created by tatiana on 26.06.2018.
//

#include "LookingForFood.h"
#include "Condition.h"
#include "GetFedOrFeedSelfAtTable.h"
#include "game/core/Character.h"

namespace behavior
{
    LookingForFood::LookingForFood()
            :Sequence()
    {
        addNode(std::make_shared<Condition>(&LookingForFood::isLookingForFood));
        addNode(std::make_shared<GetFedOrFeedSelfAtTable>());
    }

    LookingForFood::~LookingForFood()
    {

    }

    TaskResult LookingForFood::isLookingForFood(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return (character->isLookingForFood() && !character->isStarving()) ? TaskResult::Success : TaskResult::Failure;
    }
}