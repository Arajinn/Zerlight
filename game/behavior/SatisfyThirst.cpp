//
// Created by tatiana.polozova on 08.06.2018.
//

#include "SatisfyThirst.h"
#include "Condition.h"
#include "GetDrinkOrDrinkSelf.h"
#include "game/core/Character.h"

namespace behavior
{
    SatisfyThirst::SatisfyThirst()
            :Sequence()
    {
        addNode(std::shared_ptr<Condition>(new Condition(&SatisfyThirst::thirsty)));
        addNode(std::shared_ptr<GetDrinkOrDrinkSelf>(new GetDrinkOrDrinkSelf));
    }

    SatisfyThirst::~SatisfyThirst()
    {

    }

    TaskResult SatisfyThirst::thirsty(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return character->isThirsty() ? TaskResult::Success : TaskResult::Failure;
    }
}