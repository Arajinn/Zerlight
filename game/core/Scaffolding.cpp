//
// Created by tatiana on 2018-04-20.
//

#include "Scaffolding.h"

namespace game
{
    Scaffolding::Scaffolding(const map::vector3& position)
            :Stairs(position)
    {

    }

    Scaffolding::~Scaffolding()
    {

    }

    std::shared_ptr<Scaffolding> Scaffolding::create(const map::vector3& position, std::string constructionID, ConstructOrientation orientation)
    {
        std::shared_ptr<Scaffolding> ptr=std::shared_ptr<Scaffolding>(new Scaffolding(position));
        ptr->init(constructionID, orientation);
        return ptr;
    }

    void Scaffolding::init(std::string constructionID, ConstructOrientation orientation)
    {
        Stairs::init(constructionID,orientation);
    }
}