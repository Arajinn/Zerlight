//
// Created by tatiana.polozova on 09.06.2018.
//

#ifndef ZERL_ITEMHISTORY_H
#define ZERL_ITEMHISTORY_H

#include "enums.h"
#include <string>

namespace game {
    class ItemHistory {
    public:
        ItemHistory(std::string aItemID, std::string aMaterialID, std::string aName="");
        ~ItemHistory();

        const std::string& itemID() const;
        const std::string& materialID() const;

        const ItemQuality& quality() const;
    private:
        unsigned int mID;

        std::string mItemID;
        std::string mMaterialID;

        std::string mName;

        ItemQuality mQuality;
    };
}

#endif //ZERL_ITEMHISTORY_H
