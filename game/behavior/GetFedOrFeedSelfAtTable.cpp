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
        addNode(std::make_shared<GetFed>());
        addNode(std::make_shared<FindFoodAndEatAtTable>());
    }

    GetFedOrFeedSelfAtTable::~GetFedOrFeedSelfAtTable()
    {

    }
}