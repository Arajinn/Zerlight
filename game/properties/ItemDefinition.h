//
// Created by tatiana.polozova on 09.06.2018.
//

#ifndef ZERL_ITEMDEFINITION_H
#define ZERL_ITEMDEFINITION_H

#include "game/properties/ItemSettings.h"

#include <vector>
#include <string>

namespace properties {
    struct ItemDefinition {
    public:
        ItemDefinition();
        ~ItemDefinition();

        std::string ID;
        std::string Name;

        float EquippedJobPenalty;
        float EquippedMovePenalty;

        std::vector<ItemEffect> Effects;
        void addEffect(game::ItemEffectType a_effect, float a_amount);

        bool hasEffect(game::ItemEffectType effectType);

        float effectAmount(game::ItemEffectType type);
        static float effectModifier(game::ItemQuality quality);
        static float effectModifierForFoodAndDrink(game::ItemQuality quality);

        float CombatRatingModifier;
    };
}

#endif //ZERL_ITEMDEFINITION_H
