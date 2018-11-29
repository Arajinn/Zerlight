//
// Created by tatiana.polozova on 08.06.2018.
//

#include "GetDrinkOrDrinkSelf.h"
#include "GetFed.h"
#include "FindDrink.h"

namespace behavior
{
    GetDrinkOrDrinkSelf::GetDrinkOrDrinkSelf()
            :Selector()
    {
        addNode(std::shared_ptr<GetFed>(new GetFed()));
        addNode(std::shared_ptr<FindDrink>(new FindDrink()));
    }

    GetDrinkOrDrinkSelf::~GetDrinkOrDrinkSelf()
    {

    }
}