//
// Created by tatiana on 26.12.18.
//

#ifndef ZERLIGHT_TARGETEDATTACKDEF_H
#define ZERLIGHT_TARGETEDATTACKDEF_H

#include "game/core/enums.h"

#include <vector>
#include <memory>

namespace properties
{
    struct AttackDef;
}

namespace properties {
    struct TargetedAttackDef {
        std::vector<game::MaterialType> TargetedMaterials;
        std::shared_ptr<AttackDef> TargetedAttackDef{nullptr};

        bool isContain(const game::MaterialType& material) const;
    };
}

#endif //ZERLIGHT_TARGETEDATTACKDEF_H
