//
// Created by tatiana.polozova on 27.03.2018.
//

#include "MapCell.h"
#include "Map.h"
#include "Region.h"
#include "game/core/GameManager.h"
#include "game/core/Construction.h"
#include "game/core/Stairs.h"
#include "game/core/Scaffolding.h"
#include "game/core/Grass.h"
#include "game/core/Ramp.h"
#include "game/core/Liquid.h"
#include "game/core/ConstructedTerrain.h"
#include "game/properties/GameDefinition.h"
#include "game/properties/TerrainSettings.h"

#include <algorithm>

namespace map
{
    MapCell::MapCell()
    {
        mEmbeddedWall= nullptr;
        mEmbeddedFloor= nullptr;

        mRamp=nullptr;
        mLiquid=nullptr;

        mProposedJob=nullptr;

        mNavGraphNode=nullptr;

        this->updateWeight();
    }

    MapCell::MapCell(const map::vector3& position, const std::string& type)
    {
        mWall=GAME->gameDefinition()->indexOfMaterial(type);
        mFloor=GAME->gameDefinition()->indexOfMaterial(type);

        mEmbeddedWall= nullptr;
        mEmbeddedFloor= nullptr;

        mRamp=nullptr;
        mLiquid=nullptr;

        mPosition=position;

        this->updateWeight();
    }

    MapCell::MapCell(const map::vector3& position, const std::string& wallType, const std::string& floorType)
    {
        mWall=GAME->gameDefinition()->indexOfMaterial(wallType);
        mFloor=GAME->gameDefinition()->indexOfMaterial(floorType);

        mEmbeddedWall= nullptr;
        mEmbeddedFloor= nullptr;

        mRamp=nullptr;
        mLiquid=nullptr;

        mPosition=position;

        this->updateWeight();
    }

    MapCell::~MapCell()
    {
        mEmbeddedWall= nullptr;
        mEmbeddedFloor= nullptr;

        mRamp=nullptr;
        mLiquid=nullptr;
    }

    bool MapCell::hasNaturalWall() const
    {
        auto gd=GAME->gameDefinition();
        return (mWall!=gd->indexOfMaterial(gd->terrainSettings()->AirMaterialID));
    }

    bool MapCell::hasEmbeddedWall() const
    {
        return (mEmbeddedWall!= nullptr);
    }

    bool MapCell::blocksMovement() const
    {
        if (this->hasNaturalWall())
            return true;

        if (!this->hasEmbeddedWall())
            return false;

        auto castEmbeddedWall=std::dynamic_pointer_cast<game::Construction>(mEmbeddedWall);

        if (castEmbeddedWall== nullptr)
            return false;

        return castEmbeddedWall->hasFlag(game::ConstructionProperty::BlocksMovement);
    }

    bool MapCell::hasNaturalFloor() const
    {
        auto gd=GAME->gameDefinition();
        return (mFloor!=gd->indexOfMaterial(gd->terrainSettings()->AirMaterialID));
    }

    bool MapCell::hasStairs() const
    {
        if (mEmbeddedWall== nullptr)
            return false;

        auto castEmbeddedWall=std::dynamic_pointer_cast<game::Stairs>(mEmbeddedWall);
        if (castEmbeddedWall== nullptr)
            return false;

        return (castEmbeddedWall->position()==mPosition);
    }

    bool MapCell::hasEmbeddedFloor() const
    {
        return (mEmbeddedFloor!= nullptr);
    }

    void MapCell::setEmbeddedWall(std::shared_ptr<game::GameEntity> ent)
    {
        mEmbeddedWall=ent;
    }

    void MapCell::setEmbeddedFloor(std::shared_ptr<game::GameEntity> ent)
    {
        mEmbeddedFloor=ent;
    }

    bool MapCell::hasGrass() const
    {
        auto castEmbeddedFloor=std::dynamic_pointer_cast<game::Grass>(mEmbeddedFloor);
        return (castEmbeddedFloor!= nullptr);
    }

    std::shared_ptr<game::Job> MapCell::proposedJob() const
    {
        return mProposedJob;
    }

    bool MapCell::hasGrassRampTop() const
    {
        if ((this->hasGrass()) && (mRamp!= nullptr))
        {
            return (mRamp->cell()->position() != this->position());
        }

        return false;
    }

    bool MapCell::hasFloor() const
    {
        if (this->hasNaturalFloor())
            return true;

        if (this->hasEmbeddedFloor())
            return (!this->hasGrassRampTop());

        return false;
    }

    std::shared_ptr<map::NavGraphNode> MapCell::navGraphNode() const
    {
        return mNavGraphNode;
    }

    bool MapCell::willSuffocate() const
    {
        if (mLiquid== nullptr)
            return false;

        if (mLiquid->amount()<0.75f)
            return false;

        return true;
    }

    bool MapCell::hasLava() const
    {
        if (mLiquid== nullptr)
            return false;

        return (mLiquid->materialID()==GAME->gameDefinition()->terrainSettings()->LavaMaterialID);
    }

    bool MapCell::hasRamp() const
    {
        if (mRamp== nullptr)
            return false;

        return (mRamp->cell()->position()!=this->position());
    }

    void MapCell::addObject(std::shared_ptr<game::GameEntity> ent)
    {
        auto iter=std::find_if(std::begin(mObjects),std::end(mObjects),[&ent](std::shared_ptr<game::GameEntity> const& value)
        {
            return ent->ID()==value->ID();
        });

        if (iter==std::end(mObjects))
            mObjects.push_back(ent);
    }

    void MapCell::removeObject(std::shared_ptr<game::GameEntity> ent)
    {
        auto iter=std::find_if(std::begin(mObjects),std::end(mObjects),[&ent](std::shared_ptr<game::GameEntity> const& value)
        {
            return ent->ID()==value->ID();
        });

        if (iter!=std::end(mObjects))
            mObjects.erase(iter);
    }

    void MapCell::addCharacter(std::shared_ptr<game::Character> character)
    {
        auto iter=std::find_if(std::begin(mCharacters),std::end(mCharacters),[&character](std::shared_ptr<game::Character> const& value)
        {
            return character->ID()==value->ID();
        });

        if (iter==std::end(mCharacters))
            mCharacters.push_back(character);
    }

    void MapCell::removeCharacter(std::shared_ptr<game::Character> character)
    {
        auto iter=std::find_if(std::begin(mCharacters),std::end(mCharacters),[&character](std::shared_ptr<game::Character> const& value)
        {
            return character->ID()==value->ID();
        });

        if (iter==std::end(mCharacters))
            mCharacters.erase(iter);

        if ((mCharacters.size()!=0) || (mEmbeddedWall== nullptr))
            return;
    }

    bool MapCell::isWalkable() const
    {
        if (this->blocksMovement())
            return false;

        if (this->hasNaturalFloor())
            return true;

        auto upper_cell=GAME->region()->map()->cell(mPosition.x(),mPosition.y(),mPosition.z()+1);

        if (upper_cell==nullptr)
            return false;

        if (upper_cell->hasStairs()==true)
            return true;

        if (upper_cell->hasEmbeddedFloor()==false)
            return false;

        return true;
    }

    void MapCell::updateWeight()
    {
        if (!this->isWalkable())
            mWeight=1000.0f;
        else
            mWeight=0.0f;
    }

    const float& MapCell::weight() const
    {
        return mWeight;
    }

    std::shared_ptr<MapCell> MapCell::getShiftCell(int dx, int dy, int dz)
    {
        return GAME->region()->map()->cell(mPosition.x()+dx,mPosition.y()+dy,mPosition.z()+dz);
    }

    std::shared_ptr<game::GameEntity> MapCell::embeddedWall() const
    {
        return mEmbeddedWall;
    }

    void MapCell::setNaturalFloor(const int& floor)
    {
        mFloor=floor;
    }

    void  MapCell::setNavGraphNode(std::shared_ptr<map::NavGraphNode> node)
    {
        mNavGraphNode=node;
    }

    bool MapCell::hasSupport() const
    {
        if (this->hasNaturalWall())
            return true;

        if (mEmbeddedWall== nullptr)
            return false;

        auto castConstructedTerrain=std::dynamic_pointer_cast<game::ConstructedTerrain>(mEmbeddedWall);
        return (castConstructedTerrain!= nullptr);
    }

    bool MapCell::hasScaffolding() const
    {
        if (mEmbeddedWall== nullptr)
            return false;

        auto castEmbeddedWall=std::dynamic_pointer_cast<game::Scaffolding>(mEmbeddedWall);
        return (castEmbeddedWall!= nullptr);
    }

    std::shared_ptr<game::Liquid> MapCell::liquid()
    {
        return mLiquid;
    }

    const map::vector3& MapCell::position() const
    {
        return mPosition;
    }
}