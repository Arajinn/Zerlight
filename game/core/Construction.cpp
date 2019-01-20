//
// Created by tatiana on 27.03.2018.
//

#include "Construction.h"
#include "defines.h"
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

    std::shared_ptr<Construction> Construction::create(const map::vector3& position, std::string constructionID,
            ConstructOrientation orientation)
    {
        auto ptr=std::make_shared<Construction>(position);
        ptr->init(constructionID, orientation);
        return ptr;
    }

    void Construction::init(std::string constructionID, ConstructOrientation orientation)
    {
        mConstructionID=constructionID;
        mOrientation=orientation;

        mConstructionDef=GAME_DEFINITIONS->constructionDefinition(constructionID);
    }

    void Construction::spawn(std::shared_ptr<map::MapCell> mapCell)
    {
        mapCell->setEmbeddedWall(std::dynamic_pointer_cast<Construction>(shared_from_this()));

        if (mConstructionDef->hasFlag(ConstructionProperty::BlocksMovement))
        {
            WORLD_MAP->navGraph()->removeTile(mapCell);

            auto liquid=mapCell->liquid();
            if (liquid!= nullptr)
                GMINSTANCE->addToDeleteList(std::dynamic_pointer_cast<GameEntity>(liquid));
        }

        //GameEntity::spawn(mapCell);
    }

    bool Construction::hasFlag(ConstructionProperty prop) const
    {
        auto iter=std::find_if(mProperties.begin(),mProperties.end(),[&prop](ConstructionProperty const& elem)
        {
           return (elem==prop);
        });

        return (iter!=mProperties.end());
    }

    bool Construction::isUpdatable() const
    {
        return false;
    }
}