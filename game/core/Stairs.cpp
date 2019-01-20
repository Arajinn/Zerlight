//
// Created by tatiana on 27.03.2018.
//

#include "Stairs.h"
#include "defines.h"
#include "game/core/GameManager.h"
#include "game/map/MapCell.h"
#include "game/map/Region.h"
#include "game/map/Map.h"
#include "game/map/NavGraph.h"
#include "game/properties/GameDefinition.h"
#include "game/properties/TerrainSettings.h"

namespace game
{
    Stairs::Stairs(const map::vector3& position)
            :Construction(position)
    {

    }

    Stairs::~Stairs()
    {

    }
    std::shared_ptr<Stairs> Stairs::create(const map::vector3& position, std::string constructionID, ConstructOrientation orientation)
    {
        auto ptr=std::make_shared<Stairs>(position);
        ptr->init(constructionID, orientation);
        return ptr;
    }

    void Stairs::init(std::string constructionID, ConstructOrientation orientation)
    {
        Construction::init(constructionID,orientation);
    }

    void Stairs::spawn(std::shared_ptr<map::MapCell> mapCell)
    {
        auto map=WORLD_MAP;

        auto down_cell=map->cell(position().x(),position().y(),position().z()-1);
        if (down_cell!=nullptr)
        {
            down_cell->setEmbeddedWall(shared_from_this());
        }

        Construction::spawn(mapCell);

        if (down_cell!=nullptr)
        {
            if (down_cell->navGraphNode() == nullptr)
            {
                map->navGraph()->addTile(down_cell);
            }
        }

        auto this_cell=map->cell(position());
        if (this_cell!= nullptr)
        {
            map->navGraph()->reestablishConnections(this_cell);
        }
    }

    void Stairs::pre_delete()
    {
        auto map=WORLD_MAP;

        auto down_dell=map->cell(position().x(),position().y(),position().z()-1);
        down_dell->setEmbeddedWall(nullptr);

        auto this_cell=map->cell(position());
        this_cell->setEmbeddedWall(nullptr);

        Construction::pre_delete();
    }
}