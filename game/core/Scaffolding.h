//
// Created by tatiana.polozova on 2018-04-20.
//

#ifndef ZERL_SCAFFOLDING_H
#define ZERL_SCAFFOLDING_H

#include "game/core/Stairs.h"

namespace game {
    class Scaffolding : public Stairs {
    public:
        Scaffolding(const map::vector3& position);
        ~Scaffolding();

        static std::shared_ptr<Scaffolding> create(const map::vector3& position, std::string constructionID, ConstructOrientation orientation=ConstructOrientation::FrontLeft);
    protected:
        void init(std::string constructionID, ConstructOrientation orientation);
    private:

    };
}

#endif //ZERL_SCAFFOLDING_H
