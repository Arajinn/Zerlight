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
#include "game/core/CreateWorldOptions.h"
#include "game/properties/GameDefinition.h"
#include "game/properties/GameSettings.h"
#include "game/properties/TerrainSettings.h"
#include "game/properties/MaterialDef.h"
#include "game/map_generator/const.h"
#include "game/map_generator/cache.h"
#include "game/map_generator/gradient.h"
#include "game/map_generator/scalebias.h"
#include "game/map_generator/select.h"
#include "game/map_generator/riggedmultifractal.h"
#include "game/map_generator/perlin.h"
#include "game/map_generator/scale.h"
#include "game/map_generator/turbulence.h"
#include "game/map_generator/multiply.h"
#include "game/utils/Randomizer.h"

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

    bool Map::generateMap(std::shared_ptr<game::CreateWorldOptions> worldOptions)
    {
        const int air_id = GAME_DEFINITIONS->indexOfMaterial(TERRAIN_SETTINGS->AirMaterialID);
        const int dirt_id = GAME_DEFINITIONS->indexOfMaterial(TERRAIN_SETTINGS->DirtMaterialID);
        const int clay_id = GAME_DEFINITIONS->indexOfMaterial(TERRAIN_SETTINGS->ClayMaterialID);
        const int water_id = GAME_DEFINITIONS->indexOfMaterial(TERRAIN_SETTINGS->WaterMaterialID);
        const int lava_id = GAME_DEFINITIONS->indexOfMaterial(TERRAIN_SETTINGS->LavaMaterialID);
        const int grass_id = GAME_DEFINITIONS->indexOfMaterial(TERRAIN_SETTINGS->GrownOnMaterialID);

        std::vector<int> stone_ids;
        for (const auto& stone_material : TERRAIN_SETTINGS->StoneMaterialIDs)
        {
            if (stone_material=="Ceramic")
                continue;

            if (stone_material=="Brick")
                continue;

            if (stone_material=="Glass")
                continue;

            if (stone_material=="Basalt")
                continue;

            stone_ids.push_back(GAME_DEFINITIONS->indexOfMaterial(stone_material));
        }

        std::shared_ptr<noise_generator::Const> air_const = std::make_shared<noise_generator::Const>(double(air_id));
        std::shared_ptr<noise_generator::Const> dirt_const = std::make_shared<noise_generator::Const>(double(dirt_id));
        std::shared_ptr<noise_generator::Const> clay_const = std::make_shared<noise_generator::Const>(double(clay_id));
        std::shared_ptr<noise_generator::Const> water_const = std::make_shared<noise_generator::Const>(double(water_id));

        std::vector<std::shared_ptr<noise_generator::Const>> stone_items(stone_ids.size());
        for (size_t i=0,isize=stone_ids.size();i<isize;i++)
            stone_items[i]= std::make_shared<noise_generator::Const>(double(stone_ids.at(i)));

        std::shared_ptr<noise_generator::Cache> cache1;
        {
            auto gradient = std::make_shared<noise_generator::Gradient>(0.0, 0.0, 0.0, 0.0, 0.0, 0.5);
            auto scalebias = std::make_shared<noise_generator::ScaleBias>(0.5, 0.5, gradient);
            cache1 = std::make_shared<noise_generator::Cache>(scalebias);
        }

        const size_t lenght = 25;
        std::vector<std::shared_ptr<noise_generator::Select>> select_array(lenght);
        double value = 1.0;
        double value_delta = (value - 0.3) / double(lenght - 1);
        for (size_t i = 0; i < lenght; i++)
        {
            int rand = RANDOMIZER->rand(0, (int)stone_ids.size() - 1);
            auto input1 = stone_items.at((size_t)rand);

            std::shared_ptr<noise_generator::ModuleBase> input2;
            if (i == 0)
            {
                int rand0 = RANDOMIZER->rand(0, (int)stone_ids.size() - 1);
                input2 = stone_items.at((size_t)rand0);
            }
            else
            {
                input2 = select_array.at(i - 1);
            }

            select_array[i] = std::make_shared<noise_generator::Select>(value, 1.10000002384186, 0.0, input1, input2, cache1);
            value -= value_delta;
        }

        const int rm = 0;

        std::shared_ptr<noise_generator::Select> select_dirt_clay;
        {
            int rand_rm1 = RANDOMIZER->rand(0, rm);
            auto rigged_multifractal1 = std::make_shared<noise_generator::RiggedMultifractal>(6.0, 2.0, 4, rand_rm1);
            auto scale_bias1 = std::make_shared<noise_generator::ScaleBias>(0.5, 0.5, rigged_multifractal1);
            select_dirt_clay = std::make_shared<noise_generator::Select>(0.95, 1.10000002384186, 0.0, dirt_const, clay_const, scale_bias1);
        }

        std::shared_ptr<noise_generator::Select> select_all;
        {
            int rand_rm2 = RANDOMIZER->rand(0, rm);
            auto rigged_multifractal2 = std::make_shared<noise_generator::RiggedMultifractal>(1.0, 2.0, 4, rand_rm2);
            auto scale_bias2 = std::make_shared<noise_generator::ScaleBias>(0.5, 0.5, rigged_multifractal2);
            auto select0 = std::make_shared<noise_generator::Select>(0.95, 1.10000002384186, 0.0, select_dirt_clay, water_const, scale_bias2);
            auto select2 = std::make_shared<noise_generator::Select>(0.25, 1.10000002384186, 0.0, select0, select_array.back(), cache1);
            select_all = std::make_shared<noise_generator::Select>(0.2, 1.10000002384186, 0.0, air_const, select2, cache1);
        }

        std::shared_ptr<noise_generator::Cache> cache2;
        {
            int seed = RANDOMIZER->rand(0, rm);
            auto perlin = std::make_shared<noise_generator::Perlin>(worldOptions->GroundFrequency, 2.0, 0.5, worldOptions->GroundOctave, seed);
            auto scale = std::make_shared<noise_generator::Scale>(1.0, 1.0, 0.899999976158142, perlin);
            cache2 = std::make_shared<noise_generator::Cache>(scale);
        }

        std::shared_ptr<noise_generator::Turbulence> turbulence;
        {
            auto multiply = std::make_shared<noise_generator::Multiply>(cache2, cache2);
            turbulence = std::make_shared<noise_generator::Turbulence>(nullptr, nullptr, multiply, 1.0, 1.0, worldOptions->GroundHeight, select_all);
        }

        std::shared_ptr<noise_generator::ScaleBias> scale_bias;
        {
            auto perlin1 = std::make_shared<noise_generator::Perlin>(3.0, 2.0, 0.5, 3, 1001);
            auto perlin2 = std::make_shared<noise_generator::Perlin>(3.0, 2.0, 0.5, 3, 1201);
            auto perlin3 = std::make_shared<noise_generator::Perlin>(3.0, 2.0, 0.5, 3, 1301);
            auto rigged_multifractal1 = std::make_shared<noise_generator::RiggedMultifractal>(2.0, 2.0, 1, 0);
            auto const0 = std::make_shared<noise_generator::Const>(0.0);
            auto const1 = std::make_shared<noise_generator::Const>(1.0);
            auto select1 = std::make_shared<noise_generator::Select>(-0.2, 1.1, 0.0, const0, const1, rigged_multifractal1);
            auto rigged_multifractal2 = std::make_shared<noise_generator::RiggedMultifractal>(2.0, 2.0, 1, 1323);
            auto select2 = std::make_shared<noise_generator::Select>(-0.2, 1.1, 0.0, const0, const1, rigged_multifractal2);
            auto multiply1 = std::make_shared<noise_generator::Multiply>(select1, select2);
            auto multiply2 = std::make_shared<noise_generator::Multiply>(multiply1, cache1);
            auto turbulence1 = std::make_shared<noise_generator::Turbulence>(perlin1, perlin2, perlin3, 0.25, 0.25, 0.25, multiply2);
            scale_bias = std::make_shared<noise_generator::ScaleBias>(-1.0, 2.0, turbulence1);
        }

        auto select = std::make_shared<noise_generator::Select>(0.5, 1.10000002384186, 0.0, air_const, turbulence, scale_bias);

        mMapWidth=worldOptions->SizeX;
        mMapHeight=worldOptions->SizeY;
        mMapDepth=worldOptions->SizeZ;

        size_t centerX = mMapWidth / 2 - worldOptions->OffsetX;
        size_t centerY = mMapHeight / 2 - worldOptions->OffsetY;

        mMap.resize(mMapWidth);
        for (size_t x=0;x<mMapWidth;x++)
        {
            mMap[x].resize(mMapHeight);
            for (size_t y = 0; y < mMapHeight; y++)
            {
                mMap[x][y].resize(mMapDepth);
                std::fill(mMap[x][y].begin(),mMap[x][y].end(),nullptr);
            }
        }

        const double coeff=128.0;
        for (int z=mMapDepth-1;z>=0;z--)
        {
            for (int y = 0; y < mMapHeight; y++)
            {
                for (int x=0;x<mMapWidth;x++)
                {
                    //map::vector3 position(x,y,mMapDepth-z-1);
                    map::vector3 position(x,y,z);

                    std::shared_ptr<MapCell> mapCell=nullptr;

                    std::shared_ptr<MapCell> upperCell=nullptr;
                    if (z<mMapDepth-1)
                        //upperCell=mMap[x][y][mMapDepth-z];
                        upperCell=mMap[x][y][z+1];

                    int type=air_id;

                    if (z==mMapDepth-1)
                    {
                        mapCell=std::make_shared<MapCell>(position,type);
                    }
                    else
                    {
                        const double input_x=(double(x)-double(centerX))/coeff;
                        const double input_y=(double(y)-double(centerY))/coeff;
                        const double input_z=double(z)/coeff;
                        const float value=select->getValue(input_x,input_y,input_z);
                        type=int(value);
                        mapCell=std::make_shared<MapCell>(position,type);
                    }

                    //mMap[x][y][mMapDepth-z-1]=mapCell;
                    mMap[x][y][z]=mapCell;

                    if (type==water_id)
                    {
                        if (z<26)
                        {
                            mapCell->setNaturalWallAndFloor(air_id);
                        }
                        else if (z==26)
                        {
                            mapCell->setNaturalWall(air_id);
                            mapCell->setNaturalFloor(dirt_id);
                            //TODO spawn water
                        }
                        else if (z==27)
                        {
                            mapCell->setNaturalWall(air_id);
                            mapCell->setNaturalFloor(dirt_id);
                            if ((upperCell!=nullptr) && (upperCell->liquid()!=nullptr))
                            {
                                upperCell->setNaturalFloor(air_id);
                                //TODO remove mud
                            }
                            //TODO spawn water
                        }
                        else if (z==28)
                        {
                            mapCell->setNaturalWallAndFloor(dirt_id);
                            //TODO create ramp
                        }
                        else
                        {
                            mapCell->setNaturalWallAndFloor(dirt_id);
                        }
                    }
                    else if (z==mMapDepth-1)
                    {
                        mapCell->setNaturalWallAndFloor(air_id);
                        //TODO Lava spawn
                    }
                    else if ((upperCell!=nullptr) && (upperCell->isAir()))
                    {
                        upperCell->setNaturalFloor(type);
                        //TODO create ramp
                    }
                    else if (z>=125)
                    {
                        //TODO Lava spawn
                    }

                    if ((upperCell==nullptr) || ((upperCell->isAir()) && (upperCell->isOutside())))
                        mapCell->setOutside(true);
                }
            }
        }

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
                        new_cell=std::make_shared<MapCell>(position,"Granite");
                    }
                    else if (z>21)
                    {
                        new_cell=std::make_shared<MapCell>(position,"Air");
                    }
                    else if (z==20)
                    {
                        if ((x<20) && (y<20))
                        //if (x<20)
                        {
                            new_cell = std::make_shared<MapCell>(position, "Granite");
                        }
                        else
                            {
                            new_cell = std::make_shared<MapCell>(position, "Air", "Granite");
                        }
                    }
                    else if (z==21)
                    {
                        if ((x<20) && (y<20))
                        //if (x<20)
                        {
                            new_cell = std::make_shared<MapCell>(position, "Air", "Granite");
                        }
                        else
                        {
                            new_cell=std::make_shared<MapCell>(position,"Air");
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

    const size_t& Map::mapWidth() const
    {
        return mMapWidth;
    }

    const size_t& Map::mapHeight() const
    {
        return mMapHeight;
    }

    const size_t& Map::mapDepth() const
    {
        return mMapDepth;
    }

    void Map::postInit()
    {
        mNavGraph=std::make_shared<NavGraph>();
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
            if ((delta_cell!=nullptr) && (!delta_cell->isVisible()) &&
                    (((delta_pos.z()>=pos.z()) && (!delta_cell->hasFloor())) ||
                    ((delta_pos.z()<=pos.z()) && (!pos_cell->hasFloor()))))
            {
                delta_cell->setIsVisible(true);
                init_count++;

                if (!delta_cell->hasFloor())
                    source.push_back(delta_pos);
            }
        }

        while (!source.empty())
        {
            pos=source.back();
            source.erase(source.end());
            pos_cell=cell(pos);
            for (auto delta:deltas)
            {
                auto delta_pos=pos+delta;
                auto delta_cell=cell(delta_pos);
                if ((delta_cell!=nullptr) && (!delta_cell->isVisible()) &&
                    (((delta_pos.z()>=pos.z()) && (!delta_cell->hasFloor())) ||
                     ((delta_pos.z()<=pos.z()) && (!pos_cell->hasFloor()))))
                {
                    delta_cell->setIsVisible(true);
                    init_count++;
                    mInitProgress=0.5f+float(init_count)/float(cells_count)/2.0f;

                    if (!delta_cell->hasFloor())
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