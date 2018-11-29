//
// Created by tatiana.polozova on 27.03.2018.
//

#include "Region.h"
#include "Map.h"
#include "game/core/Fortress.h"

namespace map
{
    Region::Region()
    {
        mMap=std::shared_ptr<Map>(new Map());
        mFortress=std::shared_ptr<game::Fortress>(new game::Fortress());
    }

    Region::~Region()
    {

    }

    const std::shared_ptr<Map> Region::map() const
    {
        return mMap;
    }

    const std::shared_ptr<game::Fortress> Region::fortress() const
    {
        return mFortress;
    }

    void Region::processSpawn()
    {
        mFortress->processSpawn();
    }

    void Region::processDeletion()
    {
        mFortress->processDeletion();
    }
}