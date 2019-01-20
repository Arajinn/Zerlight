//
// Created by tatiana on 23.12.18.
//

#include "PickupWeapon.h"
#include "Condition.h"
#include "Action.h"
#include "game/core/defines.h"
#include "game/core/Character.h"
#include "game/core/Item.h"
#include "game/core/Weapon.h"
#include "game/map/MapCell.h"
#include "game/properties/ItemDefinition.h"
#include "game/properties/WeaponDef.h"
#include "game/utils/Randomizer.h"

namespace behavior {
    PickupWeapon::PickupWeapon()
            :Sequence()
    {
        addNode(std::make_shared<Condition>(&PickupWeapon::haveTarget));
        addNode(std::make_shared<Condition>(&PickupWeapon::canPickupWeapon));
        addNode(std::make_shared<Action>(&PickupWeapon::pickupRandomWeapon));
    }

    PickupWeapon::~PickupWeapon()
    {

    }

    TaskResult PickupWeapon::haveTarget(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return ((character->currentTarget()!= nullptr) && (!character->shouldRunFromTarget())) ? TaskResult::Success : TaskResult::Failure;
    }

    TaskResult PickupWeapon::canPickupWeapon(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return ((character->canPickupWeapon()) && (character->cell()->getObjectsCount()>0)) ? TaskResult::Success : TaskResult::Failure;
    }

    TaskResult PickupWeapon::pickupRandomWeapon(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        auto cell=character->cell();
        int objectsCount=cell->getObjectsCount();
        int randObjectIndex=RANDOMIZER->rand(0,objectsCount-1);
        std::shared_ptr<game::Item> item=std::dynamic_pointer_cast<game::Item>(cell->getObjects().at(randObjectIndex));
        if (item!= nullptr)
        {
            if (character->canCarry(item))
            {
                const auto itemDef=GAME_DEFINITIONS->itemDefinition(item->itemID());
                if (itemDef->WeaponSize>0.0f)
                {
                    if (character->heldItems().empty())
                    {
                        const auto natural_weapons=character->naturalWeapons();
                        for (auto weapon : natural_weapons)
                        {
                            if (character->skillLevel(weapon->weaponDef()->Skill)>character->skillLevel(itemDef->ItemWeaponDef->Skill))
                                return TaskResult::Failure;
                        }
                    }

                    character->pickupItem(item);
                    return TaskResult::Success;
                }
            }
        }

        return TaskResult::Failure;
    }
}