//
// Created by tatiana on 10.01.19.
//

#include "CreateWorldOptions.h"

namespace game {
    CreateWorldOptions::CreateWorldOptions(size_t aSizeX,size_t aSizeY,size_t aSizeZ)
    {
        SizeX=aSizeX;
        SizeY=aSizeY;
        SizeZ=aSizeZ;

        OffsetX=0;
        OffsetY=0;

        KingdomName="";

        Seed=0;

        GroundHeight=0.1;
        GroundFrequency=1.75;
        GroundOctave=2;
    }
}