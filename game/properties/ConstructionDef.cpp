//
// Created by tatiana.polozova on 2018-04-20.
//

#include "ConstructionDef.h"
#include <algorithm>

namespace properties {
    ConstructionDef::ConstructionDef()
    {

    }

    ConstructionDef::~ConstructionDef()
    {

    }

    bool ConstructionDef::hasFlag(game::ConstructionProperty flag)
    {
        auto iter=std::find_if(std::begin(Properties),std::end(Properties),[&flag](game::ConstructionProperty const& value)
        {
            return (flag==value);
        });

        return (iter!=std::end(Properties));
    }
}