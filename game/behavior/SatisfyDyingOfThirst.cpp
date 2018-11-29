//
// Created by tatiana.polozova on 6/15/2018.
//

#include "SatisfyDyingOfThirst.h"
#include "Condition.h"
#include "GetDrinkOrDrinkSelf.h"
#include "game/core/Character.h"

namespace behavior
{
    SatisfyDyingOfThirst::SatisfyDyingOfThirst()
            :Sequence()
    {
        addNode(std::shared_ptr<Condition>(new Condition(&SatisfyDyingOfThirst::dyingOfThirst)));
        addNode(std::shared_ptr<GetDrinkOrDrinkSelf>(new GetDrinkOrDrinkSelf));
    }

    SatisfyDyingOfThirst::~SatisfyDyingOfThirst()
    {

    }

    TaskResult SatisfyDyingOfThirst::dyingOfThirst(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return (character->isDyingOfThirst() || character->isLookingForDrink()) ? TaskResult::Success : TaskResult::Failure;
    }
}