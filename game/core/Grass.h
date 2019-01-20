//
// Created by tatiana on 27.03.2018.
//

#ifndef ZERL_GRASS_H
#define ZERL_GRASS_H

#include "game/core/GameEntity.h"

namespace game {
    class Grass : public GameEntity {
    public:
        Grass(const map::vector3& position);
        virtual ~Grass();

    };
}

#endif //ZERL_GRASS_H
