//
// Created by tatiana on 2018-04-19.
//

#include "ConstructedTerrain.h"

namespace game
{
    ConstructedTerrain::ConstructedTerrain(const map::vector3 &position)
            :Construction(position)
    {

    }

    ConstructedTerrain::~ConstructedTerrain()
    {

    }

    std::shared_ptr<ConstructedTerrain> ConstructedTerrain::create(const map::vector3& position, std::string constructionID, ConstructOrientation orientation)
    {
        std::shared_ptr<ConstructedTerrain> ptr=std::shared_ptr<ConstructedTerrain>(new ConstructedTerrain(position));
        ptr->init(constructionID, orientation);
        return ptr;
    }

    void ConstructedTerrain::init(std::string constructionID, ConstructOrientation orientation)
    {
        Construction::init(constructionID,orientation);
    }
}