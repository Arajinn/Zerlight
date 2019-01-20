//
// Created by tatiana on 08.06.2018.
//

#include "GetDrinkOrDrinkSelf.h"
#include "GetFed.h"
#include "FindDrink.h"

namespace behavior
{
    GetDrinkOrDrinkSelf::GetDrinkOrDrinkSelf()
            :Selector()
    {
        addNode(std::make_shared<GetFed>());
        addNode(std::make_shared<FindDrink>());
    }

    GetDrinkOrDrinkSelf::~GetDrinkOrDrinkSelf()
    {

    }
}