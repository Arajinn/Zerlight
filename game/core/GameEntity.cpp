//
// Created by tatiana on 26.03.2018.
//

#include "GameEntity.h"
#include "GameManager.h"
#include "Item.h"
#include "Construction.h"
#include "defines.h"
#include "game/properties/GameDefinition.h"
#include "game/properties/ItemDefinition.h"
#include "game/map/Region.h"
#include "game/map/Map.h"
#include "game/map/MapCell.h"

#include <iostream>

namespace game
{
    GameEntity::GameEntity(const map::vector3& position)
            :mPosition(position)
            ,mPosition0(position)
            ,mValid(true)
            ,mToBeMoved(false)
    {

    }

    GameEntity::~GameEntity()
    {

    }

    bool GameEntity::entityHasEffect(std::shared_ptr<GameEntity> ent, ItemEffectType type)
    {
        auto castItem=std::dynamic_pointer_cast<game::Item>(ent);

        if (castItem!= nullptr)
        {
            return GAME_DEFINITIONS->itemDefinition(castItem->itemID())->hasEffect(type);
        }
        else
        {
            auto castConstruction=std::dynamic_pointer_cast<game::Construction>(ent);
            if (castConstruction!= nullptr)
            {

            }
        }

        return false;
    }

    map::vector3 GameEntity::position() const
    {
        return mPosition0;
    }

    map::vector3 GameEntity::new_position() const
    {
        return mPosition;
    }

    void GameEntity::update_position()
    {
        std::cout << "from " << mPosition0.x() << " " << mPosition0.y() << " " << mPosition0.z() << " to " << mPosition.x() << " " << mPosition.y() << " " << mPosition.z() << std::endl;
        mPosition0=mPosition;
        mToBeMoved=false;
    }

    void GameEntity::pre_update()
    {

    }

    void GameEntity::update(float dt)
    {

    }

    void GameEntity::post_update()
    {

    }

    void GameEntity::moveTo(map::vector3 new_position)
    {
        if (new_position!=mPosition0)
        {
            mPosition = new_position;
            mToBeMoved = true;
        }
    }

    const bool& GameEntity::toBeMoved() const
    {
        return mToBeMoved;
    }

    std::shared_ptr<map::MapCell> GameEntity::cell() const
    {
        return WORLD_MAP->cell(mPosition0);
    }

    void GameEntity::spawn(std::shared_ptr<map::MapCell> mapCell)
    {
        mapCell->addObject(shared_from_this());
    }

    void GameEntity::destroy(std::shared_ptr<map::MapCell> mapCell)
    {
        mapCell->removeObject(shared_from_this());
    }

    bool GameEntity::isValid()
    {
        if (mGameObjectID!=-1)
            return mValid;
        else
            return false;
    }

    void GameEntity::setID(const int& value)
    {
        mGameObjectID=value;
    }

    const int& GameEntity::ID() const
    {
        return mGameObjectID;
    }

    void GameEntity::pre_delete()
    {
        mValid=false;
    }

    float GameEntity::effectAmount(ItemEffectType type)
    {
        return 0.0f;
    }

    std::vector<properties::TileDef> GameEntity::get_draw_info(const int& x, const int& y, const int& z,
                                                     const view_orientation& orientation) const
    {
        return std::vector<properties::TileDef>();
    }
}
