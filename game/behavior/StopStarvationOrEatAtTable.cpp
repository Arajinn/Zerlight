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
        addNode(std::shared_ptr<LookingForFood>(new LookingForFood));
        addNode(std::shared_ptr<GetFedOrFeedSelf>(new GetFedOrFeedSelf));
    }

    StopStarvationOrEatAtTable::~StopStarvationOrEatAtTable()
    {

    }
}