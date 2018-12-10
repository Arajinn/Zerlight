//
// Created by tatiana on 26.06.2018.
//

#include "GetFedOrFeedSelfAtTable.h"
#include "GetFed.h"
#include "FindFoodAndEatAtTable.h"

namespace behavior
{
    GetFedOrFeedSelfAtTable::GetFedOrFeedSelfAtTable()
            :Selector()
    {
        addNode(std::shared_ptr<GetFed>(new GetFed));
        addNode(std::shared_ptr<FindFoodAndEatAtTable>(new FindFoodAndEatAtTable));
    }

    GetFedOrFeedSelfAtTable::~GetFedOrFeedSelfAtTable()
    {

    }
}