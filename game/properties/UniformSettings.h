//
// Created by tatiana on 24.12.18.
//

#ifndef ZERLIGHT_UNIFORMSETTINGS_H
#define ZERLIGHT_UNIFORMSETTINGS_H

#include <vector>
#include <string>

namespace properties {
    struct UniformSettings {
    public:
        UniformSettings();
        ~UniformSettings();

        bool isArmor(const std::string& itemID) const;
        bool isWeapon(const std::string& itemID) const;
        bool isShield(const std::string& itemID) const;
    private:
        std::vector<std::string> mArmor;
        std::vector<std::string> mWeapons;
        std::vector<std::string> mShield;
    };
}

#endif //ZERLIGHT_UNIFORMSETTINGS_H
