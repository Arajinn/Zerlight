//
// Created by tatiana on 26.03.2018.
//

#ifndef ZERL_RACEDEFINITION_H
#define ZERL_RACEDEFINITION_H

#include "game/properties/AttributeDef.h"

#include <string>
#include <vector>

namespace properties {
    struct RaceDefinition {
    public:
        RaceDefinition();
        ~RaceDefinition();

        std::string ID;
        std::string Name;
        std::string BodyID;

        float MoveSpeed;

        float TiredLevel;
        float RestTime;
        float ExhaustionTime;

        float ThirstRate;
        float ThirstLevel;
        float DyingOfThirstLevel;
        float DrinkRatio;

        float HungerRate;
        float StarvationLevel;
        float HungerLevel;
        float FoodRatio;

        float BloodLossRate;

        std::vector<AttributeDef> Attributes;
        std::vector<std::pair<std::string,float>> AdditionalDiet;

        float additionalDietValue(std::string itemID) const;
    };
}

#endif //ZERL_RACEDEFINITION_H
