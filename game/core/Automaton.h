//
// Created by tatiana on 06.01.19.
//

#ifndef ZERLIGHT_AUTOMATON_H
#define ZERLIGHT_AUTOMATON_H

#include "game/core/Character.h"

namespace game {
    class Automaton : public Character{
    public:
        Automaton(const map::vector3& position);
        virtual ~Automaton();
    };
}

#endif //ZERLIGHT_AUTOMATON_H
