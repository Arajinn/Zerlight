//
// Created by tatiana on 09.06.2018.
//

#include "ItemSettings.h"
#include "game/core/Item.h"
#include "game/core/StringConstants.h"

#include <algorithm>

namespace properties
{
    ItemEffect::ItemEffect()
    {
        Amount=0.0f;
        Effect=game::ItemEffectType ::None;
    }

    ItemEffect::ItemEffect(game::ItemEffectType a_effect, float a_amount)
    {
        Amount=a_amount;
        Effect=a_effect;
    }

    ItemEffect::~ItemEffect()
    {

    }

    ItemsByMaterial::ItemsByMaterial()
    {

    }

    ItemsByMaterial::~ItemsByMaterial()
    {

    }

    bool ItemsByMaterial::isItemInStocks(std::shared_ptr<game::Item> item)
    {
        std::string materialID=item->materialID();

        auto iter_material=std::find_if(items.begin(),items.end(),[&materialID](ItemsByMaterialInfo const& value)
        {
            return (value.materialID==materialID);
        });

        if (iter_material==items.end())
            return false;
        else
        {
            auto iter=std::find_if(iter_material->items.begin(),iter_material->items.end(),[&item](std::shared_ptr<game::Item> const& value)
            {
                return (value->ID()==item->ID());
            });

            return !(iter==iter_material->items.end());
        }
    }

    ItemsByMaterial ItemsByQualityInfo::itemsOfQualityOrHigher(game::ItemQuality itemQuality)
    {
        if (itemQuality==game::ItemQuality::Any)
            itemQuality=game::ItemQuality::Poor;

        ItemsByMaterial result;

        for (auto item_by_quality : items)
        {
            if (item_by_quality.quality>=itemQuality)
            {
                for (auto info : item_by_quality.items.items)
                {
                    result.items.push_back(info);
                }
            }
        }

        return result;
    }

    ItemSettings::ItemSettings()
    {

    }

    ItemSettings::~ItemSettings()
    {

    }

    std::vector<std::string> ItemSettings::containersByAmmoItemID(const std::string& id) const
    {
        auto iter=std::find_if(mAmmoContainersByAmmoItemID.begin(),mAmmoContainersByAmmoItemID.end(),[&id](AmmoContainerByAmmoItemID const& item)
        {
            return item.ammoItemID==id;
        });

        if (iter!=mAmmoContainersByAmmoItemID.end())
            return iter->ammoContainers;
        else
            return std::vector<std::string>();
    }

    std::string ItemSettings::itemIDToAmmoID(const std::string& itemID) const
    {
        auto iter=std::find_if(mItemIDToAmmoID.begin(),mItemIDToAmmoID.end(),[&itemID](ItemIDToAmmoID const& item)
        {
            return item.itemID==itemID;
        });

        if (iter!=mItemIDToAmmoID.end())
            return iter->ammoID;
        else
            return AMMOID_NONE;
    }

    std::string ItemSettings::bodyPartToItemID(const std::string& bodyPartID) const
    {
        auto iter=std::find_if(mBodyPartIDToItemID.begin(),mBodyPartIDToItemID.end(),[&bodyPartID](BodyPartIDToItemID const& item)
        {
            return item.bodyPartID==bodyPartID;
        });

        if (iter!=mBodyPartIDToItemID.end())
            return iter->itemID;
        else
            return ITEMID_NONE;
    }
}