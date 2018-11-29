//
// Created by tatiana.polozova on 26.03.2018.
//

#include "Squad.h"
#include "Formation.h"
#include <limits>

namespace game
{
    Squad::Squad()
            :mFormation(nullptr)
    {

    }

    Squad::~Squad()
    {

    }

    unsigned int Squad::positionIndex(const Character* member)
    {
        if (member==nullptr)
            return std::numeric_limits<unsigned int>::max();

        for (int i=0,isize=mMembers.size();i<isize;i++)
        {
            if (mMembers.at(i).get()==member)
                return i;
        }

        return std::numeric_limits<unsigned int>::max();
    }

    std::shared_ptr<SquadPosition> Squad::positionByIndex(unsigned int posIndex)
    {
        if (mFormation== nullptr)
            return nullptr;

        if (posIndex>=mFormation->positions.size())
            return nullptr;

        return mFormation->positions.at(posIndex);
    }

    std::shared_ptr<SquadPosition> Squad::position(const Character* member)
    {
        return this->positionByIndex(this->positionIndex(member));
    }

    float Squad::formationMoveSpeedBonus() const
    {
        return 1.0f;
    }
}