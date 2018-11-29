//
// Created by tatiana.polozova on 27.03.2018.
//

#include "Construction.h"
#include "GameManager.h"
#include "Liquid.h"
#include "game/properties/GameDefinition.h"
#include "game/properties/ConstructionDef.h"
#include "game/map/MapCell.h"
#include "game/map/Map.h"
#include "game/map/Region.h"
#include "game/map/NavGraph.h"

#include <algorithm>
#include <memory>

namespace game
{
    Construction::Construction(const map::vector3& position)
            :GameEntity(position)
    {

    }

    Construction::~Construction()
    {

    }

    std::shared_ptr<Construction> Construction::create(const map::vector3& position, std::string constructionID, ConstructOrientation orientation)
    {
        std::shared_ptr<Construction> ptr=std::shared_ptr<Construction>(new Construction(position));
        ptr->init(constructionID, orientation);
        return ptr;
    }

    void Construction::init(std::string constructionID, ConstructOrientation orientation)
    {
        mConstructionID=constructionID;
        mOrientation=orientation;

        mConstructionDef=GAME->gameDefinition()->constructionDefinition(constructionID);
    }

    void Construction::spawn(std::shared_ptr<map::MapCell> mapCell)
    {
        mapCell->setEmbeddedWall(std::dynamic_pointer_cast<Construction>(shared_from_this()));

        if (mConstructionDef->hasFlag(ConstructionProperty::BlocksMovement)==true)
        {
            GAME->region()->map()->navGraph()->removeTile(mapCell);

            auto liquid=mapCell->liquid();
            if (liquid!= nullptr)
                GAME->addToDeleteList(std::dynamic_pointer_cast<GameEntity>(liquid));
        }

        GameEntity::spawn(mapCell);
    }

    bool Construction::hasFlag(ConstructionProperty prop) const
    {
        auto iter=std::find_if(std::begin(mProperties),std::end(mProperties),[&prop](ConstructionProperty const& value)
        {
           return (value==prop);
        });

        return (iter!=std::end(mProperties));
    }
}