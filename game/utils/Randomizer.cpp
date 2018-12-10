//
// Created by tatiana on 05.04.2018.
//

#include "Randomizer.h"

#define _USE_MATH_DEFINES
#include "math.h"

namespace utils {
    Randomizer::Randomizer()
            :gen(std::random_device()())
    {

    }

    Randomizer::~Randomizer()
    {

    }

    std::shared_ptr<Randomizer> Randomizer::instance()
    {
        static std::shared_ptr<Randomizer> self;

        if (!self)
        {
            self = std::shared_ptr<Randomizer>(new Randomizer());
        }

        return self;
    }

    float Randomizer::norm(float min, float max, float mean, float coeff)
    {
        float sigma=(mean-min)/coeff;
        std::normal_distribution<float> dis(0.0,sigma);

        float randomValue = dis(gen);
        float result;
        if (randomValue<0.0f)
        {
            result=mean+randomValue;
        }
        else
        {
            result=mean+(max-mean)/(mean-min)*randomValue;
        }

        return result;
    }
}