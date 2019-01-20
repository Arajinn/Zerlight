//
// Created by tatiana on 06.01.19.
//

#include "HandleTarget.h"
#include "RunFromTarget.h"
#include "EngageTarget.h"

namespace behavior {
    HandleTarget::HandleTarget()
        :Selector()
    {
        addNode(std::make_shared<RunFromTarget>());
        addNode(std::make_shared<EngageTarget>());
    }

    HandleTarget::~HandleTarget()
    {

    }
}