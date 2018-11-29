//
// Created by tatiana.polozova on 27.03.2018.
//

#include "Map.h"
#include "MapCell.h"
#include "NavGraph.h"
#include "game/core/Stairs.h"
#include "game/core/GameManager.h"
#include "game/properties/GameDefinition.h"
#include "game/properties/GameSettings.h"

namespace map
{
    Map::Map()
        :mNavGraph(nullptr)
    {

    }

    Map::~Map()
    {

    }

    void Map::testMap()
    {
        mMapWidth=64;
        mMapHeight=64;
        mMapDepth=64;
        mMap.resize(mMapWidth);
        for (int x=0;x<mMapWidth;x++)
        {
            mMap[x].resize(mMapHeight);
            for (int y=0;y<mMapHeight;y++)
            {
                mMap[x][y].resize(mMapDepth);
                for (int z=0;z<mMapDepth;z++)
                {
                    map::vector3 position(x,y,z);

//                    if (z<20)
//                        mMap[x][y][z]=std::shared_ptr<MapCell>(new MapCell(position,"Granite"));
//                    else if (z==20)
//                        mMap[x][y][z]=std::shared_ptr<MapCell>(new MapCell(position,"Air","Granite"));
//                    else
//                        mMap[x][y][z]=std::shared_ptr<MapCell>(new MapCell(position,"Air"));

                    if (z<20)
                    {
                        mMap[x][y][z]=std::shared_ptr<MapCell>(new MapCell(position,"Granite"));
                    }
                    else if (z>21)
                    {
                        mMap[x][y][z]=std::shared_ptr<MapCell>(new MapCell(position,"Air"));
                    }
                    else if (z==20)
                    {
                        if (x<20) {
                            mMap[x][y][z] = std::shared_ptr<MapCell>(new MapCell(position, "Granite"));
                        }
                        else if (x==20)
                        {
                            mMap[x][y][z] = std::shared_ptr<MapCell>(new MapCell(position, "Air", "Granite"));

                            //std::shared_ptr<game::Stairs> stairs=std::shared_ptr<game::Stairs>(new game::Stairs(position,"SoilStairs"));
                            //GAME->addToSpawnList(stairs);
                        }
                        else {
                            mMap[x][y][z] = std::shared_ptr<MapCell>(new MapCell(position, "Air", "Granite"));
                        }
                    }
                    else if (z==21)
                    {
                        if (x<20)
                        {
                            mMap[x][y][z] = std::shared_ptr<MapCell>(new MapCell(position, "Air", "Granite"));
                        }
//                        else if (x==20)
//                        {
//                            mMap[x][y][z]=std::shared_ptr<MapCell>(new MapCell(position,"Air"));
//
//                            std::shared_ptr<game::Stairs> stairs=std::shared_ptr<game::Stairs>(new game::Stairs(position,"SoilStairs"));
//                            GAME->addToSpawnList(stairs);
//                        }
                        else
                        {
                            mMap[x][y][z]=std::shared_ptr<MapCell>(new MapCell(position,"Air"));
                        }
                    }
                    else
                    {
                        //achtung
                        int t=0;
                    }
                }
            }
        }
    }

    bool Map::inBounds(int x, int y, int z) const
    {
        return ((z>=0) && (z<mMapDepth) && (x>=0) && (x<mMapWidth) && (y>=0) && (y<mMapHeight));
    }

    bool Map::inBounds(const vector3& pos) const
    {
        return this->inBounds(pos.x(),pos.y(),pos.z());
    }

    const std::shared_ptr<MapCell> Map::cell(const vector3& pos) const
    {
        if (this->inBounds(pos))
            return mMap[pos.x()][pos.y()][pos.z()];
        else
            return nullptr;
    }

    const std::shared_ptr<MapCell> Map::cell(const int& x,const int& y,const int& z) const
    {
        if (this->inBounds(x,y,z))
            return mMap[x][y][z];
        else
            return nullptr;
    }

    bool Map::isWalkable(const vector3& pos) const
    {
        auto pos_cell=this->cell(pos);

        if (pos_cell== nullptr)
            return false;

        return pos_cell->isWalkable();
    }

    const int& Map::mapWidth() const
    {
        return mMapWidth;
    }

    const int& Map::mapHeight() const
    {
        return mMapHeight;
    }

    const int& Map::mapDepth() const
    {
        return mMapDepth;
    }

    void Map::postInit()
    {
        mNavGraph=std::shared_ptr<NavGraph>(new NavGraph());
    }

    const std::shared_ptr<NavGraph> Map::navGraph()
    {
        return mNavGraph;
    }

    float Map::distanceSquaredWithBias(const vector3& pos1, const vector3& pos2)
    {
        float deltaX=float(pos1.x()-pos2.x());
        float deltaY=float(pos1.y()-pos2.y());
        float deltaZ=float(pos1.z()-pos2.z())*GAME->gameDefinition()->gameSettings()->DepthDistanceBias;

        return (deltaX*deltaX+deltaY*deltaY+deltaZ*deltaZ);
    }
}