//
// Created by tatiana on 27.03.2018.
//

#ifndef ZERL_STAIRS_H
#define ZERL_STAIRS_H

#include "game/core/Construction.h"

namespace game{
    class Stairs : public Construction{
    public:
        Stairs(const map::vector3& position);
        virtual ~Stairs();

        static std::shared_ptr<Stairs> create(const map::vector3& position, std::string constructionID, ConstructOrientation orientation=ConstructOrientation::FrontLeft);

        virtual void spawn(std::shared_ptr<map::MapCell> mapCell) override;
        virtual void pre_delete() override;

    protected:
        void init(std::string constructionID, ConstructOrientation orientation);
    };
}

#endif //ZERL_STAIRS_H
