//
// Created by tatiana on 09.06.2018.
//

#ifndef ZERL_ITEMHISTORY_H
#define ZERL_ITEMHISTORY_H

#include "game/core/enums.h"
#include "game/core/types.h"

#include <string>
#include <vector>
#include <memory>

namespace game
{
    class Item;
    class Character;
    class CharacterHistory;
}

namespace game {
    class ItemHistory {
    public:
        ItemHistory(std::shared_ptr<Item> aItem, std::string aItemID, std::string aMaterialID, std::string aName="");
        ~ItemHistory();

        void pre_delete();

        const std::string& itemID() const;
        const MaterialID_t& materialID() const;

        const ItemQuality& quality() const;

        std::string materialName() const;

        bool hasUniqueName() const;
        std::string name() const;

        MaterialID_t materialAtIndex(const int& index) const;

        std::vector<std::shared_ptr<Item>> components() const;
        void clearComponents();

        void killed(std::shared_ptr<Character> victim);
    private:
        std::shared_ptr<Item> mItem;

        std::string mItemID;
        MaterialID_t mMaterialID;

        std::string mName;

        ItemQuality mQuality;

        std::vector<std::shared_ptr<Item>> mComponents;
        std::shared_ptr<CharacterHistory> mCrafterHistory;

        std::vector<std::shared_ptr<CharacterHistory>> mKills;
    };
}

#endif //ZERL_ITEMHISTORY_H
