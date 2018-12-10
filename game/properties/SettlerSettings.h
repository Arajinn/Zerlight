//
// Created by tatiana on 13.06.2018.
//

#ifndef ZERL_SETTLERSETTINGS_H
#define ZERL_SETTLERSETTINGS_H

#include "game/map/vector3.h"

#include <string>

namespace properties {
    struct SettlerSettings {
    public:
        SettlerSettings();
        ~SettlerSettings();

        map::vector3 offset;
        std::string RaceID;
        std::string Profession;
    };
}

#endif //ZERL_SETTLERSETTINGS_H
