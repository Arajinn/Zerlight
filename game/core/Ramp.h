//
// Created by tatiana on 28.03.2018.
//

#ifndef ZERL_RAMP_H
#define ZERL_RAMP_H

#include "game/core/Construction.h"

namespace game {
    class Ramp : public Construction{
    public:
        Ramp(const map::vector3& position);
        ~Ramp();

        static std::shared_ptr<Ramp> create(const map::vector3& position, std::string constructionID, const int& materialIndex,
                ConstructOrientation orientation=ConstructOrientation::FrontLeft);

        virtual void spawn(std::shared_ptr<map::MapCell> mapCell) override;

        int materialIndex() const;

        virtual std::vector<properties::TileDef> get_draw_info(const int& x, const int& y, const int& z,
                                                     const view_orientation& orientation) const;
    protected:
        void init(std::string constructionID, const int& materialIndex, ConstructOrientation orientation);
    private:
        int mMaterialIndex;
    };
}

#endif //ZERL_RAMP_H
