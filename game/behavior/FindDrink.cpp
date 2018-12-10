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
        addNode(std::shared_ptr<LookForDrink>(new LookForDrink()));
        addNode(std::shared_ptr<DrinkItem>(new DrinkItem()));
    }

    FindDrink::~FindDrink()
    {

    }
}