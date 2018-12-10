//
// Created by tatiana on 09.06.2018.
//

#include "ItemHistory.h"

namespace game
{
    ItemHistory::ItemHistory(std::string aItemID, std::string aMaterialID, std::string aName)
        :mItemID(aItemID)
        ,mMaterialID(aMaterialID)
        ,mName(aName)
        ,mQuality(ItemQuality::Average)
    {

    }

    ItemHistory::~ItemHistory()
    {

    }

    const std::string& ItemHistory::itemID() const
    {
        return mItemID;
    }

    const std::string& ItemHistory::materialID() const
    {
        return mMaterialID;
    }

    const ItemQuality& ItemHistory::quality() const
    {
        return mQuality;
    }
}