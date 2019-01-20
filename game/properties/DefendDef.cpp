//
// Created by tatiana on 28.12.18.
//

#include "DefendDef.h"

#include <algorithm>

namespace properties {
    bool DefendDef::defendsAgainst(const game::AttackType& type) const
    {
        auto iter=std::find_if(DefendTypes.begin(),DefendTypes.end(),[&type](game::AttackType const& elem)
        {
            return elem==type;
        });

        return (iter!=DefendTypes.end());
    }
}