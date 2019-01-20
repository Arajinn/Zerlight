//
// Created by tatiana on 26.06.2018.
//

#include "StopStarvationOrEatAtTable.h"
#include "LookingForFood.h"
#include "GetFedOrFeedSelf.h"

namespace behavior
{
    StopStarvationOrEatAtTable::StopStarvationOrEatAtTable()
            :Selector()
    {
        addNode(std::make_shared<LookingForFood>());
        addNode(std::make_shared<GetFedOrFeedSelf>());
    }

    StopStarvationOrEatAtTable::~StopStarvationOrEatAtTable()
    {

    }
}