//
// Created by tatiana on 06.01.19.
//

#include "SenseThreats.h"
#include "Action.h"
#include "PickupWeapon.h"
#include "AttackEnemy.h"
#include "TrackEnemy.h"
#include "game/core/Character.h"

namespace behavior {
    SenseThreats::SenseThreats()
            :Selector()
    {
        addNode(std::make_shared<PickupWeapon>());
        addNode(std::make_shared<AttackEnemy>());
        addNode(std::make_shared<TrackEnemy>());
        addNode(std::make_shared<Action>(&SenseThreats::dropExtraWeapons));
    }

    SenseThreats::~SenseThreats()
    {

    }

    TaskResult SenseThreats::dropExtraWeapons(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return TaskResult::Failure;
    }
}