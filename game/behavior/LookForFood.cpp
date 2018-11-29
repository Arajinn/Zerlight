//
// Created by tatiana.polozova on 22.06.2018.
//

#include "LookForFood.h"
#include "Condition.h"
#include "Action.h"
#include "game/core/Character.h"
#include "game/core/Item.h"
#include "game/core/Job.h"

#include <iostream>

namespace behavior
{
    LookForFood::LookForFood()
            :Selector()
    {
        addNode(std::shared_ptr<Condition>(new Condition(&LookForFood::haveFood)));
        addNode(std::shared_ptr<Action>(new Action(&LookForFood::findFood)));
    }

    LookForFood::~LookForFood()
    {

    }

    TaskResult LookForFood::haveFood(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        auto heldItems=character->heldItems();
        for (auto item : heldItems)
        {
            if (game::GameEntity::entityHasEffect(item,game::ItemEffectType::Food))
            {
                if (character->job()!=nullptr)
                {
                    std::shared_ptr<game::Item> object=nullptr;
                    if (character->job()->isClaimed(item))
                        object=item;

                    character->cancelJob();

                    if (object!= nullptr)
                        character->pickupItem(object);
                }

                return TaskResult::Success;
            }
        }

        auto currentNeedGoal=character->currentNeedGoal();
        if (currentNeedGoal==nullptr)
            return TaskResult::Failure;

        if (!game::GameEntity::entityHasEffect(currentNeedGoal,game::ItemEffectType::Food))
            return TaskResult::Failure;

        if (character->job()!=nullptr)
            character->cancelJob();

        return TaskResult::Success;
    }

    TaskResult LookForFood::findFood(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        if (!character->findEntity(game::ItemEffectType::Food))
            return TaskResult::Failure;

        std::cout << "Eat!!!!!" << std::endl;
        character->setLookingForFood();

        return TaskResult::Success;
    }
}