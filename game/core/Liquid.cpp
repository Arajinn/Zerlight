//
// Created by tatiana on 28.03.2018.
//

#include "Liquid.h"

namespace game
{
    Liquid::Liquid(const map::vector3& position)
            :GameEntity(position)
            ,mAmount(1.0f)
    {

    }

    Liquid::~Liquid()
    {

    }

    const float& Liquid::amount() const
    {
        return mAmount;
    }

    const std::string& Liquid::materialID() const
    {
        return mMaterialID;
    }
}