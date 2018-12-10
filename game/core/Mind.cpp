//
// Created by tatiana on 6/15/2018.
//

#include "Mind.h"
#include "game/utils/math_utils.h"

namespace game
{
    Mind::Mind()
    {

    }

    Mind::~Mind()
    {

    }

    std::shared_ptr<Mind> Mind::create(std::shared_ptr<game::Character> parent)
    {
        std::shared_ptr<Mind> ptr=std::shared_ptr<Mind>(new Mind());
        ptr->init(parent);
        return ptr;
    }

    void Mind::init(std::shared_ptr<Character> parent)
    {
        owner=parent;
        mHappinessLevel=50.0f;
    }

    void Mind::adjustHappiness(float delta)
    {
        mHappinessLevel=std::max(utils::clamp(mHappinessLevel+delta,0.0f,100.0f),-25.0f);
    }
}