//
// Created by tatiana on 23.12.18.
//

#include "WeaponDef.h"
#include "DefendDef.h"

namespace properties
{
    std::vector<std::shared_ptr<DefendDef>> WeaponDef::defendMoves(const game::AttackType& type) const
    {
        std::vector<std::shared_ptr<DefendDef>> result;
        for (const auto& def : DefendMoves)
        {
            if (def->defendsAgainst(type))
                result.push_back(def);
        }

        return result;
    }
}