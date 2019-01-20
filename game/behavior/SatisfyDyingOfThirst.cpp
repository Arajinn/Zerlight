//
// Created by tatiana on 6/15/2018.
//

#include "SatisfyDyingOfThirst.h"
#include "Condition.h"
#include "GetDrinkOrDrinkSelf.h"
#include "game/core/Character.h"
#include "game/core/Body.h"

namespace behavior
{
    SatisfyDyingOfThirst::SatisfyDyingOfThirst()
            :Sequence()
    {
        addNode(std::make_shared<Condition>(&SatisfyDyingOfThirst::dyingOfThirst));
        addNode(std::make_shared<GetDrinkOrDrinkSelf>());
    }

    SatisfyDyingOfThirst::~SatisfyDyingOfThirst()
    {

    }

    TaskResult SatisfyDyingOfThirst::dyingOfThirst(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return (character->body()->isDyingOfThirst() || character->body()->isLookingForDrink()) ? TaskResult::Success : TaskResult::Failure;
    }
}