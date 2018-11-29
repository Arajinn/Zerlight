//
// Created by tatiana.polozova on 08.06.2018.
//

#include "PlayerCharacterBehavior.h"
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
        addNode(std::shared_ptr<SatisfyDyingOfThirst>(new SatisfyDyingOfThirst()));
        addNode(std::shared_ptr<SatisfyStarvation>(new SatisfyStarvation()));
        addNode(std::shared_ptr<SatisfyThirst>(new SatisfyThirst()));
        addNode(std::shared_ptr<SatisfyHunger>(new SatisfyHunger()));
        addNode(std::shared_ptr<WanderInGeneral>(new WanderInGeneral()));
    }

    PlayerCharacterBehavior::~PlayerCharacterBehavior()
    {

    }
}