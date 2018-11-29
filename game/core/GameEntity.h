//
// Created by tatiana.polozova on 26.03.2018.
//

#ifndef ZERL_GAMEENTITY_H
#define ZERL_GAMEENTITY_H

#include "game/core/enums.h"
#include "game/map/vector3.h"

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
        ~GameEntity();

        virtual void pre_update();
        virtual void update(float dt);
        virtual void post_update();
        virtual void moveTo(map::vector3 new_position);
        virtual void spawn(std::shared_ptr<map::MapCell> mapCell);
        virtual void destroy(std::shared_ptr<map::MapCell> mapCell);
        virtual void pre_delete();

        void setID(const int& value);
        const int& ID() const;

        map::vector3 position() const;
        map::vector3 new_position() const;
        void update_position();

        std::shared_ptr<map::MapCell> cell() const;
        bool isValid();

        virtual float effectAmount(ItemEffectType type);

        static bool entityHasEffect(std::shared_ptr<GameEntity> ent, ItemEffectType type);

        const bool& toBeMoved() const;
    protected:

    private:
        int mGameObjectID;
        bool mValid;
        map::vector3 mPosition, mPosition0;
        bool mToBeMoved;
    };
}

#endif //ZERL_GAMEENTITY_H
