//
// Created by tatiana on 27.03.2018.
//

#ifndef ZERL_MAPCELL_H
#define ZERL_MAPCELL_H

#include "game/map/vector3.h"
#include <game/core/Character.h>

#include <memory>
#include <vector>

namespace game
{
    class GameEntity;
    class Ramp;
    class Liquid;
    class Job;
    class Stairs;
    class Designation;
}

namespace map
{
    class NavGraphNode;
}

namespace map {
    class MapCell {
    public:
        MapCell();
        MapCell(const map::vector3& position, const std::string& type);
        MapCell(const map::vector3& position, const std::string& wallType, const std::string& floorType);
        MapCell(const map::vector3& position, const int& type);
        MapCell(const map::vector3& position, const int& wallType, const int& floorType);
        ~MapCell();

        void addObject(std::shared_ptr<game::GameEntity> ent);
        void removeObject(std::shared_ptr<game::GameEntity> ent);
        int getObjectsCount() const;
        std::vector<std::shared_ptr<game::GameEntity>> getObjects() const;

        void addCharacter(std::shared_ptr<game::Character> character);
        void removeCharacter(std::shared_ptr<game::Character> character);
        int getCharactersCount() const;
        std::vector<std::shared_ptr<game::Character>> getCharacters() const;

        void setEmbeddedWall(std::shared_ptr<game::GameEntity> ent);
        void setEmbeddedFloor(std::shared_ptr<game::GameEntity> ent);

        void setNaturalWallAndFloor(const int& type);
        void setNaturalFloor(const int& type);
        void setNaturalWall(const int& type);

        void setRamp(std::shared_ptr<game::GameEntity> ent);
        std::shared_ptr<const game::Ramp> ramp() const;
        bool hasRamp() const;

        std::shared_ptr<MapCell> getShiftCell(int dx, int dy, int dz);

        bool isWalkable() const;

        bool blocksMovement() const;
        bool hasNaturalWall() const;
        bool hasEmbeddedWall() const;
        bool hasNaturalFloor() const;
        bool hasEmbeddedFloor() const;
        bool hasFloor() const;
        bool hasStairs() const;
        bool hasGrass() const;
        bool hasGrassRampTop() const;
        bool willSuffocate() const;
        bool hasLava() const;
        bool hasSupport() const;
        bool hasScaffolding() const;
        bool hasWall() const;

        const float& weight() const;

        std::shared_ptr<game::GameEntity> embeddedWall() const;

        std::shared_ptr<game::Job> proposedJob() const;

        std::shared_ptr<map::NavGraphNode> navGraphNode() const;
        void  setNavGraphNode(std::shared_ptr<map::NavGraphNode> node);

        std::shared_ptr<game::Liquid> liquid();

        const map::vector3& position() const;

        bool isVisible() const;
        void setIsVisible(const bool& value);

        bool isOutside() const;
        void setOutside(const bool& value);

        bool isAir() const;

        int naturalWall() const;
        int naturalFloor() const;

        std::shared_ptr<const game::Stairs> stairs() const;

        std::shared_ptr<game::Designation> designation() const;

        void setVisibility(const float& value);
        bool visibility() const;

        bool areEnemiesVisible() const;

        bool blocksLOS() const;
        bool blocksVerticalLOS() const;
    private:
        map::vector3 mPosition;

        int mWall;
        int mFloor;

        float mWeight;
        void updateWeight();

        std::vector<std::shared_ptr<game::GameEntity>> mObjects;
        std::vector<std::shared_ptr<game::Character>> mCharacters;

        std::shared_ptr<game::GameEntity> mEmbeddedWall;
        std::shared_ptr<game::GameEntity> mEmbeddedFloor;

        std::shared_ptr<game::Ramp> mRamp;
        std::shared_ptr<game::Liquid> mLiquid;

        std::shared_ptr<game::Job> mProposedJob;

        std::shared_ptr<map::NavGraphNode> mNavGraphNode;

        std::shared_ptr<game::Designation> mDesignation;

        bool mIsVisible, mOutside;
        float mVisibility;

        float ambientLight() const;
        float totalVisibility() const;
    };
}

#endif //ZERL_MAPCELL_H
