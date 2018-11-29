//
// Created by tatiana.polozova on 28.03.2018.
//

#ifndef ZERL_RAMP_H
#define ZERL_RAMP_H

#include "game/core/Construction.h"

namespace game {
    class Ramp : public Construction{
    public:
        Ramp(const map::vector3& position);
        ~Ramp();

        static std::shared_ptr<Ramp> create(const map::vector3& position, std::string constructionID, ConstructOrientation orientation=ConstructOrientation::FrontLeft);

    protected:
        void init(std::string constructionID, ConstructOrientation orientation);
    private:

    };
}

#endif //ZERL_RAMP_H
