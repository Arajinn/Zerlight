//
// Created by tatiana on 26.06.2018.
//

#include "GetFedOrFeedSelf.h"
#include "GetFed.h"
#include "FindFood.h"

namespace behavior
{
    GetFedOrFeedSelf::GetFedOrFeedSelf()
            :Selector()
    {
        addNode(std::make_shared<GetFed>());
        addNode(std::make_shared<FindFood>());
    }

    GetFedOrFeedSelf::~GetFedOrFeedSelf()
    {

    }
}