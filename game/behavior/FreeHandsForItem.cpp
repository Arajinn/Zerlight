//
// Created by tatiana.polozova on 22.06.2018.
//

#include "FreeHandsForItem.h"
#include "Condition.h"
#include "Action.h"
#include "game/core/Character.h"
#include "game/core/Item.h"
#include "game/core/Construction.h"
#include "game/core/Job.h"

namespace behavior
{
    FreeHandsForItem::FreeHandsForItem()
            :Selector()
    {
        addNode(std::shared_ptr<Condition>(new Condition(&FreeHandsForItem::canCarry)));
        addNode(std::shared_ptr<Action>(new Action(&FreeHandsForItem::sheathExtraItems)));
        addNode(std::shared_ptr<Action>(new Action(&FreeHandsForItem::dropExraItems)));
    }

    FreeHandsForItem::~FreeHandsForItem()
    {

    }

    TaskResult FreeHandsForItem::canCarry(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        std::shared_ptr<game::Item> object=nullptr;

        auto currentNeedGoal=character->currentNeedGoal();
        if (currentNeedGoal!= nullptr)
        {
            auto castToItem=std::dynamic_pointer_cast<game::Item>(currentNeedGoal);
            if (castToItem!= nullptr)
                object=castToItem;
            else
            {
                auto castToConstruction=std::dynamic_pointer_cast<game::Construction>(currentNeedGoal);
                if (castToConstruction!= nullptr)
                    return TaskResult::Success;
            }
        }
        else if (character->job()!=nullptr)
            object=character->job()->nextComponent();

        return ((object==nullptr) || (!character->canCarry(object))) ? TaskResult::Failure : TaskResult::Success;
    }

    TaskResult FreeHandsForItem::sheathExtraItems(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        std::shared_ptr<game::Item> object=nullptr;

        auto currentNeedGoal=character->currentNeedGoal();
        if (currentNeedGoal!= nullptr)
        {
            auto castToItem=std::dynamic_pointer_cast<game::Item>(currentNeedGoal);
            if (castToItem!= nullptr)
                object=castToItem;
            else if (character->job()!= nullptr)
                object=character->job()->nextComponent();
        }
        else if (character->job()!= nullptr)
            object=character->job()->nextComponent();

        return ((object==nullptr) || (!character->sheathItem()) || (!character->canCarry(object))) ? TaskResult::Failure : TaskResult::Success;
    }

    TaskResult FreeHandsForItem::dropExraItems(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        if (character->currentNeedGoal()==nullptr)
            return TaskResult::Failure;

        auto object=std::dynamic_pointer_cast<game::Item>(character->currentNeedGoal());
        if (object==nullptr)
        {
            if (character->job()!= nullptr)
                object=character->job()->nextComponent();
        }

        if (object== nullptr)
            return TaskResult::Failure;

        bool flag=false;

        if (!character->canCarry(game::EquipmentType::RightHand))
            flag=character->dropItem(game::EquipmentType::RightHand);

        if ((!flag) || (!character->canCarry(game::EquipmentType::LeftHand)))
            flag=flag || character->dropItem(game::EquipmentType::LeftHand);

        return (flag) ? TaskResult::Success : TaskResult::Failure;
    }
}