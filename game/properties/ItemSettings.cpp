//
// Created by tatiana.polozova on 09.06.2018.
//

#include "ItemSettings.h"
#include "game/core/Item.h"

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

        auto iter_material=std::find_if(std::begin(items),std::end(items),[&materialID](ItemsByMaterialInfo const& value)
        {
            return (value.materialID==materialID);
        });

        if (iter_material==std::end(items))
            return false;
        else
        {
            auto iter=std::find_if(std::begin(iter_material->items),std::end(iter_material->items),[&item](std::shared_ptr<game::Item> const& value)
            {
                return (value->ID()==item->ID());
            });

            return !(iter==std::end(iter_material->items));
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
}