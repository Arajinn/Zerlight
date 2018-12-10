//
// Created by tatiana on 26.06.2018.
//

#include "FindFoodAndEatAtTable.h"
#include "LookForFood.h"
#include "GetFood.h"
#include "LookForChairThenEatAtTable.h"

namespace behavior
{
    FindFoodAndEatAtTable::FindFoodAndEatAtTable()
        :Sequence()
    {
        addNode(std::shared_ptr<LookForFood>(new LookForFood));
        addNode(std::shared_ptr<GetFood>(new GetFood));
        addNode(std::shared_ptr<LookForChairThenEatAtTable>(new LookForChairThenEatAtTable));
    }

    FindFoodAndEatAtTable::~FindFoodAndEatAtTable()
    {

    }
}