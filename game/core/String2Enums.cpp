//
// Created by tatiana on 07.12.18.
//

#include "String2Enums.h"

namespace game
{
    MaterialType String2Enums::str2MaterialType(const std::string& str)
    {
        if (str=="Air")
            return MaterialType::Air;
        else if (str=="Soil")
            return MaterialType::Soil;
        else if (str=="Stone")
            return MaterialType::Stone;
        else if (str=="Liquid")
            return MaterialType::Liquid;
        else if (str=="Wood")
            return MaterialType::Wood;
        else if (str=="Coal")
            return MaterialType::Coal;
        else if (str=="Metal")
            return MaterialType::Metal;
        else if (str=="Gem")
            return MaterialType::Gem;
        else if (str=="Plant")
            return MaterialType::Plant;
        else if (str=="Cloth")
            return MaterialType::Cloth;
        else if (str=="Bone")
            return MaterialType::Bone;
        else if (str=="Hide")
            return MaterialType::Hide;
        else if (str=="Flesh")
            return MaterialType::Flesh;
        else if (str=="BodyFluid")
            return MaterialType::BodyFluid;
        else
            return MaterialType::Undef;
    }
}