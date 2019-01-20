//
// Created by tatiana on 09.06.2018.
//

#include "defines.h"
#include "ItemHistory.h"
#include "Item.h"
#include "StringConstants.h"
#include "Character.h"
#include "CharacterHistory.h"
#include "game/properties/MaterialDef.h"

namespace game
{
    ItemHistory::ItemHistory(std::shared_ptr<Item> aItem, std::string aItemID, std::string aMaterialID, std::string aName)
        :mItemID(aItemID)
        ,mMaterialID(aMaterialID)
        ,mName(aName)
        ,mItem(aItem)
        ,mQuality(ItemQuality::Average)
    {

    }

    ItemHistory::~ItemHistory()
    {
        std::cout << "~ItemHistory()" << std::endl;
    }

    void ItemHistory::pre_delete()
    {
        mItem=nullptr;
        mComponents.clear();
        mCrafterHistory= nullptr;
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

    bool ItemHistory::hasUniqueName() const
    {
        return (mName!="");
    }

    std::string ItemHistory::materialName() const
    {
        if (!mComponents.empty())
            return mComponents.front()->materialName();

        auto materialDef=GAME_DEFINITIONS->materialDefinition(mMaterialID);
        const auto material=materialDef->Name;

        if (mCrafterHistory==nullptr)
            return material;

        if (mItemID==CROSSBOWBOLTID)
            return material;

        if (mItemID==MUSKETROUNDID)
            return material;

        if (!materialDef->OnlyAddUniqueName)
            return mCrafterHistory->name();

        if (mCrafterHistory->hasUniqueName())
            return mCrafterHistory->name()+" "+material;

        return material;
    }

    std::string ItemHistory::name() const
    {
        if (hasUniqueName())
            return mName;

        return properties::GameDefinition::itemName(mItemID,mMaterialID,mItem);
    }

    std::string ItemHistory::materialAtIndex(const int& index) const
    {
        if ((index<0) || (mComponents.empty()))
            return mMaterialID;

        return mComponents.at(index)->materialID();
    }

    std::vector<std::shared_ptr<Item>> ItemHistory::components() const
    {
        return mComponents;
    }

    void ItemHistory::clearComponents()
    {
        mComponents.clear();
    }

    void ItemHistory::killed(std::shared_ptr<Character> victim)
    {
        mKills.push_back(victim->history());

        if (hasUniqueName())
            return;

        if (mKills.size()<10)
            return;

        // TODO
    }
}