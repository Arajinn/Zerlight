//
// Created by tatiana.polozova on 05.04.2018.
//

#ifndef ZERL_RANDOMIZER_H
#define ZERL_RANDOMIZER_H

#include <memory>
#include <random>

#define RANDOMIZER utils::Randomizer::instance()

namespace utils {
    class Randomizer {
    public:
        Randomizer();
        ~Randomizer();

        static std::shared_ptr<Randomizer> instance();

        float norm(float min, float max, float mean, float coeff=3.0f);
    private:
        std::mt19937 gen;
        //std::normal_distribution<float> dis;
    };
}

#endif //ZERL_RANDOMIZER_H
