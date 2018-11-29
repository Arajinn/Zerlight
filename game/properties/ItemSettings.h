//
// Created by tatiana.polozova on 09.06.2018.
//

#ifndef ZERL_ITEMSETTINGS_H
#define ZERL_ITEMSETTINGS_H

#include "game/core/enums.h"
//#include <string>
//#include <memory>
#include <vector>
#include <memory>

namespace game
{
    class Item;
}

namespace properties {
    struct ItemEffect
    {
    public:
        ItemEffect();
        ItemEffect(game::ItemEffectType a_effect, float a_amount);
        ~ItemEffect();

        float Amount;
        game::ItemEffectType Effect;
    };

    struct ItemsByMaterialInfo
    {
    public:
        std::string materialID;
        std::vector<std::shared_ptr<game::Item>> items;
    };

    struct ItemsByMaterial
    {
    public:
        ItemsByMaterial();
        ~ItemsByMaterial();

        std::vector<ItemsByMaterialInfo> items;

        bool isItemInStocks(std::shared_ptr<game::Item> item);
    };

    struct ItemsByQuality
    {
    public:
        game::ItemQuality quality;
        ItemsByMaterial items;
    };

    struct ItemsByQualityInfo
    {
    public:
        std::string itemID;
        std::vector<ItemsByQuality> items;

        ItemsByMaterial itemsOfQualityOrHigher(game::ItemQuality itemQuality);
    };

    struct ItemSettings {
    public:

    };
}

#endif //ZERL_ITEMSETTINGS_H
