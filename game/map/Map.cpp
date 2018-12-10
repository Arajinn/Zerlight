//
// Created by tatiana on 27.03.2018.
//

#include "Map.h"
#include "game/core/defines.h"
#include "MapCell.h"
#include "NavGraph.h"
#include "game/core/Stairs.h"
#include "game/core/Ramp.h"
#include "game/core/GameManager.h"
#include "game/properties/GameDefinition.h"
#include "game/properties/GameSettings.h"
#include "game/properties/TerrainSettings.h"
#include "game/properties/MaterialDef.h"

#include <algorithm>
#include <iostream>

namespace map
{
    Map::Map()
        :mNavGraph(nullptr)
        ,mInitProgress(0.0f)
    {

    }

    Map::~Map()
    {

    }

    bool Map::testMap()
    {
        mInitProgress=0.0f;
        mMapWidth=64;
        mMapHeight=64;
        mMapDepth=32;
        int cells_count=mMapWidth*mMapHeight*mMapDepth;
        int init_count=0;
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

                    std::shared_ptr<MapCell> new_cell=nullptr;

                    if (z<20)
                    {
                        new_cell=std::shared_ptr<MapCell>(new MapCell(position,"Granite"));
                    }
                    else if (z>21)
                    {
                        new_cell=std::shared_ptr<MapCell>(new MapCell(position,"Air"));
                    }
                    else if (z==20)
                    {
                        if ((x<20) && (y<20))
                        //if (x<20)
                        {
                            new_cell = std::shared_ptr<MapCell>(new MapCell(position, "Granite"));
                        }
                        else
                            {
                            new_cell = std::shared_ptr<MapCell>(new MapCell(position, "Air", "Granite"));
                        }
                    }
                    else if (z==21)
                    {
                        if ((x<20) && (y<20))
                        //if (x<20)
                        {
                            new_cell = std::shared_ptr<MapCell>(new MapCell(position, "Air", "Granite"));
                        }
                        else
                        {
                            new_cell=std::shared_ptr<MapCell>(new MapCell(position,"Air"));
                        }
                    }
                    else
                    {
                        //achtung
                        int t=0;
                    }

                    if ((new_cell==nullptr) || (new_cell->isAir()) || (new_cell->isOutside()))
                        new_cell->setOutside(true);

                    mMap[x][y][z]=new_cell;

                    init_count++;
                    mInitProgress=float(init_count)/float(cells_count)/2.0f;
                }
            }
        }

        generateRamps();

        for (int z=mMapDepth-1;z>=0;z--)
        {
            for (int y=0;y<mMapHeight;y++)
            {
                for (int x=0;x<mMapWidth;x++)
                {
                    auto cell=mMap[x][y][z];
                    if (cell->isOutside())
                    {
                        cell->setIsVisible(true);
                        propagateVisibility(x,y,z);
                        return true;
                    }
                }
            }
        }

        return true;
    }

    float Map::getInitProgress() const
    {
        return mInitProgress;
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
        float deltaZ=float(pos1.z()-pos2.z())*GAME_SETTINGS->DepthDistanceBias;

        return (deltaX*deltaX+deltaY*deltaY+deltaZ*deltaZ);
    }

    void Map::propagateVisibility(const int& x, const int& y, const int& z)
    {
        std::vector<vector3> deltas(10);
        deltas[0]=vector3(-1,-1,0);
        deltas[1]=vector3(0,-1,0);
        deltas[2]=vector3(1,-1,0);
        deltas[3]=vector3(-1,0,0);
        deltas[4]=vector3(1,0,0);
        deltas[5]=vector3(-1,1,0);
        deltas[6]=vector3(0,1,0);
        deltas[7]=vector3(1,1,0);
        deltas[8]=vector3(0,0,1);
        deltas[9]=vector3(0,0,-1);

        mInitProgress=0.5f;
        int cells_count=mMapWidth*mMapHeight*mMapDepth;
        int init_count=0;

        auto pos=vector3(x,y,z);

        std::vector<vector3> source;
        auto pos_cell=cell(pos);
        init_count++;
        for (auto delta : deltas)
        {
            auto delta_pos=pos+delta;
            auto delta_cell=cell(delta_pos);
            if ((delta_cell!=nullptr) && (delta_cell->isVisible()==false) &&
                    (((delta_pos.z()>=pos.z()) && (delta_cell->hasFloor()==false)) ||
                    ((delta_pos.z()<=pos.z()) && (pos_cell->hasFloor()==false))))
            {
                delta_cell->setIsVisible(true);
                init_count++;

                if (delta_cell->hasFloor()==false)
                    source.push_back(delta_pos);
            }
        }

        while (source.size()>0)
        {
            pos=source.back();
            source.erase(source.end());
            pos_cell=cell(pos);
            for (auto delta:deltas)
            {
                auto delta_pos=pos+delta;
                auto delta_cell=cell(delta_pos);
                if ((delta_cell!=nullptr) && (delta_cell->isVisible()==false) &&
                    (((delta_pos.z()>=pos.z()) && (delta_cell->hasFloor()==false)) ||
                     ((delta_pos.z()<=pos.z()) && (pos_cell->hasFloor()==false))))
                {
                    delta_cell->setIsVisible(true);
                    init_count++;
                    mInitProgress=0.5f+float(init_count)/float(cells_count)/2.0f;

                    if (delta_cell->hasFloor()==false)
                    {
                        auto iter=std::find_if(source.begin(),source.end(),[&delta_pos](vector3 const& item)
                        {
                            return (delta_pos.x()==item.x()) && (delta_pos.y()==item.y()) && (delta_pos.z()==item.z());
                        });

                        if (iter==source.end())
                            source.push_back(delta_pos);
                    }
                }
            }
        }
    }

    void Map::generateRamps()
    {
        for (int x=0;x<mMapWidth;x++)
        {
            for (int y = 0; y < mMapHeight; y++)
            {
                for (int z = 0; z < mMapDepth; z++)
                {
                    std::shared_ptr<const MapCell> cell=mMap.at(x).at(y).at(z);

                    if (cell->hasNaturalWall())
                        continue;

                    if (!cell->hasNaturalFloor())
                        continue;

                    bool hasWall1=false;
                    bool hasWall2=false;
                    bool hasWall3=false;
                    bool hasWall4=false;

                    game::MaterialType materialType;
                    int materialIndex;

                    if (x>=1)
                    {
                        std::shared_ptr<const MapCell> neighbour=mMap.at(x-1).at(y).at(z);
                        if (neighbour->hasNaturalWall())
                        {
                            hasWall1 = true;
                            materialIndex=neighbour->naturalWall();
                            materialType=GAME_DEFINITIONS->materialDefinition(materialIndex)->Type;
                        }
                        else
                            hasWall1=false;
                    }

                    if (y<mMapHeight-1)
                    {
                        std::shared_ptr<const MapCell> neighbour=mMap.at(x).at(y+1).at(z);
                        if (neighbour->hasNaturalWall())
                        {
                            hasWall2 = true;
                            materialIndex=neighbour->naturalWall();
                            materialType=GAME_DEFINITIONS->materialDefinition(materialIndex)->Type;
                        }
                        else
                            hasWall2=false;
                    }

                    if (x<mMapWidth-1)
                    {
                        std::shared_ptr<const MapCell> neighbour=mMap.at(x+1).at(y).at(z);
                        if (neighbour->hasNaturalWall())
                        {
                            hasWall3 = true;
                            materialIndex=neighbour->naturalWall();
                            materialType=GAME_DEFINITIONS->materialDefinition(materialIndex)->Type;
                        }
                        else
                            hasWall3=false;
                    }

                    if (y>=1)
                    {
                        std::shared_ptr<const MapCell> neighbour=mMap.at(x).at(y-1).at(z);
                        if (neighbour->hasNaturalWall())
                        {
                            hasWall4 = true;
                            materialIndex=neighbour->naturalWall();
                            materialType=GAME_DEFINITIONS->materialDefinition(materialIndex)->Type;
                        }
                        else
                            hasWall4=false;
                    }

                    if ((!hasWall1) && (!hasWall2) && (!hasWall3) && (!hasWall4))
                        continue;
                    else
                    {
                        std::string constructionID=TERRAIN_SETTINGS->materialToRampID(materialType);
                        std::shared_ptr<game::Ramp> ramp=game::Ramp::create(map::vector3(x,y,z),constructionID,materialIndex);
                        GMINSTANCE->addToSpawnList(ramp);
                    }
                }
            }
        }
    }
}