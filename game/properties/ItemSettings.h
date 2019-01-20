//
// Created by tatiana on 09.06.2018.
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

    struct AmmoContainerByAmmoItemID
    {
        std::string ammoItemID;
        std::vector<std::string> ammoContainers;
    };

    struct ItemIDToAmmoID
    {
        std::string itemID;
        std::string ammoID;
    };

    struct BodyPartIDToItemID
    {
        std::string bodyPartID;
        std::string itemID;
    };

    struct ItemSettings {
    public:
        ItemSettings();
        ~ItemSettings();

        std::vector<std::string> containersByAmmoItemID(const std::string& id) const;
        std::string itemIDToAmmoID(const std::string& itemID) const;
        std::string bodyPartToItemID(const std::string& bodyPartID) const;

        std::string LimbItemID,CorpseItemID;
    private:
        std::vector<AmmoContainerByAmmoItemID> mAmmoContainersByAmmoItemID;
        std::vector<ItemIDToAmmoID> mItemIDToAmmoID;
        std::vector<BodyPartIDToItemID> mBodyPartIDToItemID;
    };
}

#endif //ZERL_ITEMSETTINGS_H
