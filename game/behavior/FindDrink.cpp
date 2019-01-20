//
// Created by tatiana on 08.06.2018.
//

#include "FindDrink.h"
#include "LookForDrink.h"
#include "DrinkItem.h"

namespace behavior
{
    FindDrink::FindDrink()
            :Sequence()
    {
        addNode(std::make_shared<LookForDrink>());
        addNode(std::make_shared<DrinkItem>());
    }

    FindDrink::~FindDrink()
    {

    }
}