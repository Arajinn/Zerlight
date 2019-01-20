//
// Created by tatiana on 2018-04-19.
//

#ifndef ZERL_CONSTRUCTEDTERRAIN_H
#define ZERL_CONSTRUCTEDTERRAIN_H

#include "game/core/Construction.h"

namespace game {
    class ConstructedTerrain : public Construction {
    public:
        ConstructedTerrain(const map::vector3& position);
        virtual ~ConstructedTerrain();

        static std::shared_ptr<ConstructedTerrain> create(const map::vector3& position, std::string constructionID, ConstructOrientation orientation=ConstructOrientation::FrontLeft);
    protected:
        void init(std::string constructionID, ConstructOrientation orientation);
    private:
    };
}

#endif //ZERL_CONSTRUCTEDTERRAIN_H
