//
// Created by tatiana on 25.06.2018.
//

#include "GetFood.h"
#include "PickupFood.h"
#include "Condition.h"
#include "game/core/Character.h"
#include "game/core/Item.h"

namespace behavior
{
    GetFood::GetFood()
            :Selector()
    {
        addNode(std::shared_ptr<Condition>(new Condition(&GetFood::haveFood)));
        addNode(std::shared_ptr<PickupFood>(new PickupFood));
    }

    GetFood::~GetFood()
    {

    }

    TaskResult GetFood::haveFood(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        auto list=character->heldItems();
        for (auto item : list)
        {
            if (game::GameEntity::entityHasEffect(item,game::ItemEffectType::Food))
            {
                if (character->job()!= nullptr)
                    character->cancelJob();

                return TaskResult::Success;
            }
        }

        return TaskResult::Failure;
    }
}