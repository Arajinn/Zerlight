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
        addNode(std::make_shared<LookForFood>());
        addNode(std::make_shared<GetFood>());
        addNode(std::make_shared<LookForChairThenEatAtTable>());
    }

    FindFoodAndEatAtTable::~FindFoodAndEatAtTable()
    {

    }
}