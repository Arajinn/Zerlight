//
// Created by tatiana on 25.06.2018.
//

#include "LookForChair.h"
#include "Condition.h"
#include "Action.h"
#include "game/core/Character.h"

namespace behavior
{
    LookForChair::LookForChair()
            :Selector()
    {
        addNode(std::make_shared<Condition>(&LookForChair::haveChair));
        addNode(std::make_shared<Action>(&LookForChair::findChair));
    }

    LookForChair::~LookForChair()
    {

    }

    TaskResult LookForChair::haveChair(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        if (character->currentNeedGoal()== nullptr)
            return TaskResult::Failure;

        if (!game::GameEntity::entityHasEffect(character->currentNeedGoal(),game::ItemEffectType::Sit))
            return TaskResult::Failure;

        if (character->job()!= nullptr)
            character->cancelJob();

        return TaskResult::Success;
    }

    TaskResult LookForChair::findChair(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return (character->findEntity(game::ItemEffectType::Sit)) ? TaskResult::Success : TaskResult::Failure;
    }
}