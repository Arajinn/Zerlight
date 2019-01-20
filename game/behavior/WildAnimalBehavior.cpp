//
// Created by tatiana on 09.01.19.
//

#include "WildAnimalBehavior.h"
#include "SatisfyBreathing.h"
#include "SenseThreats.h"
#include "WanderInGeneral.h"

namespace behavior {
    WildAnimalBehavior::WildAnimalBehavior()
            :Selector()
    {
        addNode(std::make_shared<SatisfyBreathing>());
        addNode(std::make_shared<SenseThreats>());
        addNode(std::make_shared<WanderInGeneral>());
    }

    WildAnimalBehavior::~WildAnimalBehavior()
    {

    }
}