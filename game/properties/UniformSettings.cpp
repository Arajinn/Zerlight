//
// Created by tatiana on 24.12.18.
//

#include "UniformSettings.h"

#include <algorithm>

#define ITEMID_ANY "ITEMID_ANY"
#define ITEMID_ANY2H "ITEMID_ANY2H"

namespace properties {
    UniformSettings::UniformSettings()
    {

    }

    UniformSettings::~UniformSettings()
    {

    }

    bool UniformSettings::isArmor(const std::string& itemID) const
    {
        auto iter=std::find_if(mArmor.begin(),mArmor.end(),[&itemID](std::string const& elem)
        {
            return itemID==elem;
        });

        return (iter!=mArmor.end());
    }

    bool UniformSettings::isWeapon(const std::string& itemID) const
    {
        auto iter=std::find_if(mWeapons.begin(),mWeapons.end(),[&itemID](std::string const& elem)
        {
            return itemID==elem;
        });

        if ((iter==mWeapons.end()) && (itemID!=ITEMID_ANY))
            return (itemID==ITEMID_ANY2H);

        return true;
    }

    bool UniformSettings::isShield(const std::string& itemID) const
    {
        auto iter=std::find_if(mShield.begin(),mShield.end(),[&itemID](std::string const& elem)
        {
            return itemID==elem;
        });

        return (iter!=mShield.end());
    }
}