//
// Created by tatiana on 26.03.2018.
//

#ifndef ZERL_RACEDEFINITION_H
#define ZERL_RACEDEFINITION_H

#include "game/properties/AttributeDef.h"
#include "game/core/enums.h"

#include <string>
#include <vector>
#include <memory>

namespace properties
{
    struct GenderDef;
}

namespace properties {
    struct RaceDefinition {
    public:
        RaceDefinition();
        ~RaceDefinition();

        std::string ID;
        std::string Name;
        std::string LanguageID;
        std::string BodyID;

        float MoveSpeed;
        float Size;
        float TiredLevel;
        float RestTime;
        float ExhaustionTime;
        float PassOutLevel;

        float ThirstRate;
        float ThirstLevel;
        float DyingOfThirstLevel;
        float DrinkRatio;

        float HungerRate;
        float StarvationLevel;
        float HungerLevel;
        float FoodRatio;

        float BloodLossRate;
        std::string HealingItemID;

        float SightRadius;
        float CastLightRadius;
        float CombatSightRadius;

        std::vector<AttributeDef> Attributes;

        std::vector<std::pair<std::string,float>> AdditionalDiet;
        float additionalDietValue(std::string itemID) const;

        std::vector<std::shared_ptr<GenderDef>> Genders;
        game::GenderType generateGender() const;
        std::shared_ptr<const GenderDef> gender(const game::GenderType& type) const;

        float BaseAttackDelay;
        float DodgeTime;

        bool ZombieVirusCarrier;

        bool Livestock;
        int PastureSpace;
        float GestationTimeMin,GestationTimeMax;
    };
}

#endif //ZERL_RACEDEFINITION_H
