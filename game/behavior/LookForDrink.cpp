//
// Created by tatiana on 08.06.2018.
//

#include "LookForDrink.h"
#include "Condition.h"
#include "Action.h"
#include "game/core/Character.h"
#include "game/core/Body.h"

#include <iostream>

namespace behavior
{
    LookForDrink::LookForDrink()
            :Selector()
    {
        addNode(std::make_shared<Condition>(&LookForDrink::haveDrink));
        addNode(std::make_shared<Action>(&LookForDrink::findDrink));
    }

    LookForDrink::~LookForDrink()
    {

    }

    TaskResult LookForDrink::haveDrink(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        auto currentNeedGoal=character->currentNeedGoal();
        if (currentNeedGoal==nullptr)
            return TaskResult::Failure;

        if (!game::GameEntity::entityHasEffect(currentNeedGoal,game::ItemEffectType::Drink))
            return TaskResult::Failure;

        if (character->job()!=nullptr)
            character->cancelJob();

        return TaskResult::Success;
    }

    TaskResult LookForDrink::findDrink(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        if (!character->findEntity(game::ItemEffectType::Drink))
            return TaskResult::Failure;

        std::cout << "Drink!!!!!" << std::endl;
        character->body()->setLookingForDrink();

        return TaskResult::Success;
    }
}