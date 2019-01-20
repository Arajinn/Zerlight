//
// Created by tatiana on 26.12.18.
//

#include "TargetedAttackDef.h"

#include <algorithm>

namespace properties {

    bool TargetedAttackDef::isContain(const game::MaterialType& material) const
    {
        auto iter=std::find_if(TargetedMaterials.begin(),TargetedMaterials.end(),[&material](game::MaterialType const& item)
        {
            return item==material;
        });

        return (iter!=TargetedMaterials.end());
    }
}