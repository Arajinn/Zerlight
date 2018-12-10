//
// Created by tatiana on 6/14/2018.
//

#include "BodyPartDef.h"

namespace properties
{
    BodyPartDef::BodyPartDef()
    {
        Symmetrical=false;
        Thickness=1.0f;
        ToHitWeight=1.0f;
        HarvestedItem="";
        HarvestedQuantity=1;
        BodyFunction=game::BodyFunction::None;
    }

    BodyPartDef::~BodyPartDef()
    {

    }
}