//
// Created by tatiana.polozova on 26.03.2018.
//

#include "RaceDefinition.h"
#include <algorithm>

namespace properties
{
    RaceDefinition::RaceDefinition()
    {

    }

    RaceDefinition::~RaceDefinition()
    {

    }

    float RaceDefinition::additionalDietValue(std::string itemID) const
    {
        auto iter=std::find_if(std::begin(AdditionalDiet),std::end(AdditionalDiet),[&itemID](std::pair<std::string,float> const& value)
        {
            return value.first==itemID;
        });

        if (iter==std::end(AdditionalDiet))
            return 0.0f;
        else
            return iter->second;
    }
}
