//
// Created by tatiana on 6/15/2018.
//

#include "Mind.h"
#include "Character.h"
#include "Squad.h"
#include "defines.h"
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
        std::shared_ptr<Mind> ptr=std::make_shared<Mind>();
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
        mHappinessLevel=std::max(math_utils::clamp(mHappinessLevel+delta,0.0f,100.0f),-25.0f);
    }

    float Mind::combatValue() const
    {
        if (owner->squad()!=nullptr)
        {
            float value=0.0f;
            const auto members=owner->squad()->members();
            for (const auto& member : members)
            {
                if (member==nullptr)
                    continue;

                if (member->ID()==owner->ID())
                    continue;

                value+=member->combatValue();
            }

            return value;
        }

        const auto factionType=owner->factionDef()->Type;
        switch (factionType)
        {
            case FactionType::Wild:
            case FactionType::EnemyCiv:
            case FactionType::EnemySmall:
            case FactionType::Subteranean:
            case FactionType::Mant:
            case FactionType::Beetle:
            case FactionType::Undead:
                return 1000.0f;
            default:
                return 0.0f;
        }
    }

    bool Mind::isUpset() const
    {
        return (mHappinessLevel<25.0f);
    }
}