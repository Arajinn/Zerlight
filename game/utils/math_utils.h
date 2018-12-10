//
// Created by tatiana on 26.03.2018.
//

#ifndef ZERL_MATH_UTILS_H
#define ZERL_MATH_UTILS_H

#include <random>

namespace utils
{
    static inline float clamp(float aValue, float aMin, float aMax)
    {
        if (aValue<aMin)
            return aMin;
        else if (aValue>aMax)
            return aMax;
        else
            return aValue;
    }


}

#endif //ZERL_MATH_UTILS_H
