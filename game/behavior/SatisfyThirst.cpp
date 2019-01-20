//
// Created by tatiana on 08.06.2018.
//

#include "SatisfyThirst.h"
#include "Condition.h"
#include "GetDrinkOrDrinkSelf.h"
#include "game/core/Character.h"
#include "game/core/Body.h"

namespace behavior
{
    SatisfyThirst::SatisfyThirst()
            :Sequence()
    {
        addNode(std::make_shared<Condition>(&SatisfyThirst::thirsty));
        addNode(std::make_shared<GetDrinkOrDrinkSelf>());
    }

    SatisfyThirst::~SatisfyThirst()
    {

    }

    TaskResult SatisfyThirst::thirsty(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return character->body()->isThirsty() ? TaskResult::Success : TaskResult::Failure;
    }
}