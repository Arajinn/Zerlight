//
// Created by tatiana.polozova on 26.06.2018.
//

#include "LookForChairThenEatAtTable.h"
#include "LookForChair.h"
#include "EatAtTable.h"

namespace behavior
{
    LookForChairThenEatAtTable::LookForChairThenEatAtTable()
        :Sequence()
    {
        addNode(std::shared_ptr<LookForChair>(new LookForChair));
        addNode(std::shared_ptr<EatAtTable>(new EatAtTable));
    }

    LookForChairThenEatAtTable::~LookForChairThenEatAtTable()
    {

    }
}