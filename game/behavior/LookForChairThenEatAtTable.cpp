//
// Created by tatiana on 26.06.2018.
//

#include "LookForChairThenEatAtTable.h"
#include "LookForChair.h"
#include "EatAtTable.h"

namespace behavior
{
    LookForChairThenEatAtTable::LookForChairThenEatAtTable()
        :Sequence()
    {
        addNode(std::make_shared<LookForChair>());
        addNode(std::make_shared<EatAtTable>());
    }

    LookForChairThenEatAtTable::~LookForChairThenEatAtTable()
    {

    }
}