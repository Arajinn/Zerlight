//
// Created by tatiana on 17.01.19.
//

#ifndef ZERLIGHT_AMBASSADOR_H
#define ZERLIGHT_AMBASSADOR_H

#include "game/core/Character.h"

namespace game {
    class Ambassador : public Character{
    public:
        Ambassador(const map::vector3& position);
        virtual ~Ambassador();
    };
}

#endif //ZERLIGHT_AMBASSADOR_H
