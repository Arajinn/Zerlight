//
// Created by tatiana on 26.03.2018.
//

#ifndef ZERL_GAMEENTITY_H
#define ZERL_GAMEENTITY_H

#include "game/core/enums.h"
#include "game/map/vector3.h"
#include "game/properties/TileDef.h"

#include <memory>
#include <vector>

namespace map
{
    class MapCell;
}

namespace game {

    class GameEntity : public std::enable_shared_from_this<GameEntity> {
    public:
        GameEntity(const map::vector3& position);
        virtual ~GameEntity();

        virtual void pre_update();
        virtual void update(float dt);
        virtual void post_update();
        virtual void moveTo(map::vector3 new_position, bool stopPathing=false);
        virtual void spawn(std::shared_ptr<map::MapCell> mapCell) = 0;
        virtual void destroy(std::shared_ptr<map::MapCell> mapCell);
        virtual void pre_delete();

        virtual void setID(const int& value);
        const int& ID() const;

        map::vector3 position() const;
        map::vector3 new_position() const;
        map::vector3 delta_position() const;
        void update_position();

        std::shared_ptr<map::MapCell> cell() const;
        bool isValid();

        virtual float effectAmount(ItemEffectType type);

        static bool entityHasEffect(std::shared_ptr<GameEntity> ent, ItemEffectType type);

        const bool& toBeMoved() const;

        virtual bool isUpdatable() const = 0;

        virtual std::vector<properties::TileDef> get_draw_info(const int& x, const int& y, const int& z,
                                                     const view_orientation& orientation) const;
     protected:

    private:
        int mGameObjectID;
        bool mValid;
        map::vector3 mPosition, mPosition0, mDeltaPosition;
        bool mToBeMoved;
    };
}

#endif //ZERL_GAMEENTITY_H
