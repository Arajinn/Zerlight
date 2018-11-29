//
// Created by tatiana.polozova on 26.06.2018.
//

#include "GetFedOrFeedSelf.h"
#include "GetFed.h"
#include "FindFood.h"

namespace behavior
{
    GetFedOrFeedSelf::GetFedOrFeedSelf()
            :Selector()
    {
        addNode(std::shared_ptr<GetFed>(new GetFed));
        addNode(std::shared_ptr<FindFood>(new FindFood));
    }

    GetFedOrFeedSelf::~GetFedOrFeedSelf()
    {

    }
}