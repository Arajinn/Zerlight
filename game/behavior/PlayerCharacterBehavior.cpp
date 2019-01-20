//
// Created by tatiana on 08.06.2018.
//

#include "PlayerCharacterBehavior.h"
#include "SenseThreats.h"
#include "SatisfyDyingOfThirst.h"
#include "SatisfyStarvation.h"
#include "SatisfyThirst.h"
#include "SatisfyHunger.h"
#include "WanderInGeneral.h"

namespace behavior
{
    PlayerCharacterBehavior::PlayerCharacterBehavior()
            :Selector()
    {
        addNode(std::make_shared<SenseThreats>());
        addNode(std::make_shared<SatisfyDyingOfThirst>());
        addNode(std::make_shared<SatisfyStarvation>());
        addNode(std::make_shared<SatisfyThirst>());
        addNode(std::make_shared<SatisfyHunger>());
        addNode(std::make_shared<WanderInGeneral>());
    }

    PlayerCharacterBehavior::~PlayerCharacterBehavior()
    {

    }
}