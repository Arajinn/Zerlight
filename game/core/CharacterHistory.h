//
// Created by tatiana on 09.06.2018.
//

#ifndef ZERL_CHARACTERHISTORY_H
#define ZERL_CHARACTERHISTORY_H

#include "game/core/enums.h"

#include <string>
#include <memory>

namespace properties
{
    struct RaceDefinition;
}

namespace game {
    class CharacterHistory {
    public:
        CharacterHistory(std::shared_ptr<const properties::RaceDefinition> aRaceDef);
        ~CharacterHistory();

        void setName(const std::string& value);

        const GenderType& gender() const;
    private:
        friend class Character;

        std::shared_ptr<const properties::RaceDefinition> raceDef;

        std::string mName;
        GenderType mGender;
    };
}

#endif //ZERL_CHARACTERHISTORY_H
