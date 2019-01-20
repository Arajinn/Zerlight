//
// Created by tatiana on 26.06.2018.
//

#include "FindFood.h"
#include "Action.h"
#include "LookForFood.h"
#include "GetFood.h"
#include "game/core/Character.h"
#include "game/core/Item.h"

namespace behavior
{
    FindFood::FindFood()
        :Sequence()
    {
        addNode(std::make_shared<LookForFood>());
        addNode(std::make_shared<GetFood>());
        addNode(std::make_shared<Action>(&FindFood::eatLikeABarbarian));

    }

    FindFood::~FindFood()
    {

    }

    TaskResult FindFood::eatLikeABarbarian(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        auto list=character->heldItems();
        for (auto item : list)
        {
            if (item->effectAmount(game::ItemEffectType::Food)>0.0f)
                character->eatFoot(item);
        }

        character->setNeedGoal(nullptr);
        return TaskResult::Success;
    }
}