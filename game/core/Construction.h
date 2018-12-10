//
// Created by tatiana on 27.03.2018.
//

#ifndef ZERL_CONSTRUCTION_H
#define ZERL_CONSTRUCTION_H

#include "game/core/GameEntity.h"
#include "game/core/enums.h"

#include <vector>

namespace properties
{
    struct ConstructionDef;
}

namespace game {
    class Construction : public GameEntity{
    public:
        Construction(const map::vector3& position);
        ~Construction();

        static std::shared_ptr<Construction> create(const map::vector3& position, std::string constructionID, ConstructOrientation orientation=ConstructOrientation::FrontLeft);

        virtual void spawn(std::shared_ptr<map::MapCell> mapCell) override;

        bool hasFlag(ConstructionProperty prop) const;

        virtual bool isUpdatable() const;
    protected:
        void init(std::string constructionID, ConstructOrientation orientation);

    private:
        std::vector<ConstructionProperty> mProperties;
        ConstructOrientation mOrientation;
        std::string mConstructionID;

        std::shared_ptr<const properties::ConstructionDef> mConstructionDef;
    };
}

#endif //ZERL_CONSTRUCTION_H
