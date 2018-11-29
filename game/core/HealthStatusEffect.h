//
// Created by tatiana.polozova on 26.03.2018.
//

#ifndef ZERL_HEALTHSTATUSEFFECT_H
#define ZERL_HEALTHSTATUSEFFECT_H

#include "game/core/enums.h"

namespace game {
    struct HealthStatusEffect {
        HealthStatusAilment Ailment;
        float Amount;
        float RecoveryRate;
    };
}

#endif //ZERL_HEALTHSTATUSEFFECT_H
