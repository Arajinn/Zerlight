//
// Created by tatiana on 26.03.2018.
//

#ifndef ZERL_MATH_UTILS_H
#define ZERL_MATH_UTILS_H

#include <random>

namespace math_utils
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

    double interpolateLinear(const double& a, const double& b, const double& position);

    double gradientNoise3D(const double& fx, const double& fy, const double& fz, const int& ix, const int& iy, const int&iz, const int& seed);

    double mapQuinticSCurve(const double& value);

    double mapCubicSCurve(const double& value);

    double gradientCoherentNoise3D(const double& x, const double& y, const double&z, const int& seed);
}

#endif //ZERL_MATH_UTILS_H
