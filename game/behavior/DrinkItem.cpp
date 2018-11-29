//
// Created by tatiana.polozova on 13.06.2018.
//

#include "DrinkItem.h"
#include "Action.h"
#include "PathToDrink.h"
#include "game/core/Item.h"
#include "game/core/Character.h"

namespace behavior
{
    DrinkItem::DrinkItem()
            :Sequence()
    {
        addNode(std::shared_ptr<PathToDrink>(new PathToDrink()));
        addNode(std::shared_ptr<Action>(new Action(&DrinkItem::drink)));
    }

    DrinkItem::~DrinkItem()
    {

    }

    TaskResult DrinkItem::drink(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        auto currentNeedGoal=character->currentNeedGoal();
        if (currentNeedGoal== nullptr)
            return TaskResult::Failure;

        auto castGoalToItem=std::dynamic_pointer_cast<game::Item>(character->currentNeedGoal());
        if (castGoalToItem!= nullptr)
        {
            bool check=character->pickupItem(castGoalToItem);
            if (check)
            {
                character->drinkItem(castGoalToItem);
                character->setNeedGoal(nullptr);
            }
            else
                return TaskResult::Failure;
        }

        return TaskResult::Success;
    }
}