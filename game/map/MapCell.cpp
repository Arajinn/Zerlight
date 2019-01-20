//
// Created by tatiana on 27.03.2018.
//

#include "MapCell.h"
#include "game/core/defines.h"
#include "Map.h"
#include "Region.h"
#include "game/core/GameManager.h"
#include "game/core/Construction.h"
#include "game/core/Stairs.h"
#include "game/core/Scaffolding.h"
#include "game/core/Grass.h"
#include "game/core/Ramp.h"
#include "game/core/Liquid.h"
#include "game/core/Stairs.h"
#include "game/core/ConstructedTerrain.h"
#include "game/core/Character.h"
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

        mIsVisible=false;
        mOutside=false;
        mVisibility=0.0f;

        mDesignation=nullptr;

        this->updateWeight();
    }

    MapCell::MapCell(const map::vector3& position, const std::string& type)
    {
        mWall=GAME_DEFINITIONS->indexOfMaterial(type);
        mFloor=GAME_DEFINITIONS->indexOfMaterial(type);

        mEmbeddedWall= nullptr;
        mEmbeddedFloor= nullptr;

        mRamp=nullptr;
        mLiquid=nullptr;

        mPosition=position;

        mIsVisible=false;
        mOutside=false;

        mDesignation=nullptr;

        this->updateWeight();
    }

    MapCell::MapCell(const map::vector3& position, const std::string& wallType, const std::string& floorType)
    {
        mWall=GAME_DEFINITIONS->indexOfMaterial(wallType);
        mFloor=GAME_DEFINITIONS->indexOfMaterial(floorType);

        mEmbeddedWall= nullptr;
        mEmbeddedFloor= nullptr;

        mRamp=nullptr;
        mLiquid=nullptr;

        mPosition=position;

        mIsVisible=false;
        mOutside=false;

        mDesignation=nullptr;

        this->updateWeight();
    }

    MapCell::~MapCell()
    {
        mEmbeddedWall= nullptr;
        mEmbeddedFloor= nullptr;

        mRamp=nullptr;
        mLiquid=nullptr;

        mDesignation=nullptr;
    }

    bool MapCell::hasNaturalWall() const
    {
        return (mWall!=GAME_DEFINITIONS->airMaterialIndex());
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
        return (mFloor!=GAME_DEFINITIONS->airMaterialIndex());
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
        std::shared_ptr<game::Grass> castEmbeddedFloor=std::dynamic_pointer_cast<game::Grass>(mEmbeddedFloor);
        return (castEmbeddedFloor!= nullptr);
    }

    std::shared_ptr<game::Job> MapCell::proposedJob() const
    {
        return mProposedJob;
    }

    bool MapCell::hasGrassRampTop() const
    {
        if ((hasGrass()) && (mRamp!= nullptr))
            return (mRamp->cell()->position() != position());

        return false;
    }

    bool MapCell::hasFloor() const
    {
        if (hasNaturalFloor())
            return true;

        if (hasEmbeddedFloor())
            return (!hasGrassRampTop());

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

        return (mLiquid->materialID()==TERRAIN_SETTINGS->LavaMaterialID);
    }

    bool MapCell::hasRamp() const
    {
        if (mRamp== nullptr)
            return false;

        return (mRamp->cell()->position()==this->position());
    }

    void MapCell::addObject(std::shared_ptr<game::GameEntity> ent)
    {
        auto iter=std::find_if(mObjects.begin(),mObjects.end(),[&ent](std::shared_ptr<game::GameEntity> const& elem)
        {
            return ent->ID()==elem->ID();
        });

        if (iter==mObjects.end())
            mObjects.push_back(ent);
    }

    void MapCell::removeObject(std::shared_ptr<game::GameEntity> ent)
    {
        auto iter=std::find_if(mObjects.begin(),mObjects.end(),[&ent](std::shared_ptr<game::GameEntity> const& elem)
        {
            return ent->ID()==elem->ID();
        });

        if (iter!=mObjects.end())
            mObjects.erase(iter);
    }

    int MapCell::getObjectsCount() const
    {
        return mObjects.size();
    }

    std::vector<std::shared_ptr<game::GameEntity>> MapCell::getObjects() const
    {
        return mObjects;
    }

    void MapCell::addCharacter(std::shared_ptr<game::Character> character)
    {
        auto iter=std::find_if(mCharacters.begin(),mCharacters.end(),
                [&character](std::shared_ptr<game::Character> const& elem)
        {
            return character->ID()==elem->ID();
        });

        if (iter==mCharacters.end())
            mCharacters.push_back(character);
    }

    void MapCell::removeCharacter(std::shared_ptr<game::Character> character)
    {
        auto iter=std::find_if(mCharacters.begin(),mCharacters.end(),[&character](std::shared_ptr<game::Character> const& elem)
        {
            return character->ID()==elem->ID();
        });

        if (iter!=mCharacters.end())
            mCharacters.erase(iter);

        if ((!mCharacters.empty()) || (mEmbeddedWall== nullptr))
            return;
    }

    int MapCell::getCharactersCount() const
    {
        return mCharacters.size();
    }

    std::vector<std::shared_ptr<game::Character>> MapCell::getCharacters() const
    {
        return mCharacters;
    }

    bool MapCell::isWalkable() const
    {
        if (this->blocksMovement())
            return false;

        if (this->hasNaturalFloor())
            return true;

        auto upper_cell=WORLD_MAP->cell(mPosition.x(),mPosition.y(),mPosition.z()+1);

        if (upper_cell==nullptr)
            return false;

        if (upper_cell->hasStairs())
            return true;

        if (!upper_cell->hasEmbeddedFloor())
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
        return WORLD_MAP->cell(mPosition.x()+dx,mPosition.y()+dy,mPosition.z()+dz);
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

    void MapCell::setIsVisible(const bool& value)
    {
        mIsVisible=value;
    }

    bool MapCell::isVisible() const
    {
        return mIsVisible;
    }

    bool MapCell::isOutside() const
    {
        return mOutside;
    }

    void MapCell::setOutside(const bool& value)
    {
        mOutside=value;
    }

    bool MapCell::hasWall() const
    {
        if (!hasNaturalWall())
            return hasEmbeddedWall();

        return true;
    }

    bool MapCell::isAir() const
    {
        if (!hasWall())
            return !hasFloor();

        return false;
    }

    int MapCell::naturalWall() const
    {
        return mWall;
    }

    int MapCell::naturalFloor() const
    {
        return mFloor;
    }

    void MapCell::setRamp(std::shared_ptr<game::GameEntity> ent)
    {
        mRamp=std::dynamic_pointer_cast<game::Ramp>(ent);
    }

    std::shared_ptr<const game::Ramp> MapCell::ramp() const
    {
        return mRamp;
    }

    std::shared_ptr<const game::Stairs> MapCell::stairs() const
    {
        return std::dynamic_pointer_cast<game::Stairs>(mEmbeddedWall);
    }

    std::shared_ptr<game::Designation> MapCell::designation() const
    {
        return mDesignation;
    }

    void MapCell::setVisibility(const float& value)
    {
        mVisibility=value;
    }

    bool MapCell::visibility() const
    {
        return mVisibility;
    }

    float MapCell::ambientLight() const
    {
        if (mOutside)
            return REGION->outsideLight();

        return REGION->insideLight();
    }

    float MapCell::totalVisibility() const
    {
        return ambientLight()+mVisibility;
    }

    bool MapCell::areEnemiesVisible() const
    {
        return (totalVisibility()>0.75f);
    }

    bool MapCell::blocksLOS() const
    {
        if (hasNaturalWall())
            return true;

        if (!hasEmbeddedWall())
            return false;

        const auto construction=std::dynamic_pointer_cast<game::Construction>(mEmbeddedWall);
        if (construction==nullptr)
            return false;

        //TODO
        return construction->hasFlag(game::ConstructionProperty::BlocksLOS);
    }

    bool MapCell::blocksVerticalLOS() const
    {
        if (blocksLOS())
            return true;

        //TODO

        if (hasNaturalFloor())
            return true;

        if ((!hasEmbeddedFloor()) || (hasGrassRampTop()))
            return false;

        const auto construction=std::dynamic_pointer_cast<game::Construction>(mEmbeddedFloor);
        if (construction==nullptr)
            return false;

        return construction->hasFlag(game::ConstructionProperty::BlocksLOS);
    }
}