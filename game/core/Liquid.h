//
// Created by tatiana on 28.03.2018.
//

#ifndef ZERL_LIQUID_H
#define ZERL_LIQUID_H

#include "game/core/GameEntity.h"

namespace game {
    class Liquid : public GameEntity{
    public:
        Liquid(const map::vector3& position);
        virtual ~Liquid();

        const float& amount() const;
        const std::string& materialID() const;
    private:
        float mAmount;
        std::string mMaterialID;
    };
}

#endif //ZERL_LIQUID_H
