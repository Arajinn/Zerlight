//
// Created by tatiana.polozova on 28.03.2018.
//

#include "Ramp.h"

namespace game
{
    Ramp::Ramp(const map::vector3& position)
            :Construction(position)
    {

    }

    Ramp::~Ramp()
    {

    }

    std::shared_ptr<Ramp> Ramp::create(const map::vector3& position, std::string constructionID, ConstructOrientation orientation)
    {
        std::shared_ptr<Ramp> ptr=std::shared_ptr<Ramp>(new Ramp(position));
        ptr->init(constructionID, orientation);
        return ptr;
    }

    void Ramp::init(std::string constructionID, ConstructOrientation orientation)
    {
        Construction::init(constructionID,orientation);
    }
}