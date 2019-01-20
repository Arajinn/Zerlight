//
// Created by tatiana on 26.03.2018.
//

#include "HealthStatusEffect.h"

namespace game
{
    HealthStatusEffect::HealthStatusEffect()
    {
        Ailment=HealthStatusAilment::Size;
        Amount=0.0f;
        RecoveryRate=true;
    }

    HealthStatusEffect::HealthStatusEffect(const HealthStatusAilment& aAilment,const float& aAmount, const float& aRecoveryRate)
    {
        Ailment=aAilment;
        Amount=aAmount;
        RecoveryRate=aRecoveryRate;
    }
}