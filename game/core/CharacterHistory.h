//
// Created by tatiana.polozova on 09.06.2018.
//

#ifndef ZERL_CHARACTERHISTORY_H
#define ZERL_CHARACTERHISTORY_H

#include <string>
#include <memory>

namespace properties
{
    struct RaceDefinition;
}

namespace game {
    class CharacterHistory {
    public:
        CharacterHistory(std::shared_ptr<properties::RaceDefinition> aRaceDef);
        ~CharacterHistory();

        void setName(const std::string& value);
    private:
        friend class Character;

        std::shared_ptr<properties::RaceDefinition> raceDef;

        std::string mName;
    };
}

#endif //ZERL_CHARACTERHISTORY_H
