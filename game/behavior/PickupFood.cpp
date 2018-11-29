//
// Created by tatiana.polozova on 25.06.2018.
//

#include "PickupFood.h"
#include "PathToFood.h"
#include "FreeHandsForItem.h"
#include "Action.h"
#include "game/core/Character.h"
#include "game/core/Item.h"

namespace behavior
{
    PickupFood::PickupFood()
            :Sequence()
    {
        addNode(std::shared_ptr<PathToFood>(new PathToFood));
        addNode(std::shared_ptr<FreeHandsForItem>(new FreeHandsForItem));
        addNode(std::shared_ptr<Action>(new Action(&PickupFood::pickup)));
    }

    PickupFood::~PickupFood()
    {

    }

    TaskResult PickupFood::pickup(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        if (character->currentNeedGoal()==nullptr)
            return TaskResult::Failure;

        auto object=std::dynamic_pointer_cast<game::Item>(character->currentNeedGoal());
        if (object== nullptr)
            return TaskResult::Failure;

        character->pickupItem(object);
        character->setNeedGoal(nullptr);
        return TaskResult::Success;
    }
}