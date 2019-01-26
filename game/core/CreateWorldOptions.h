//
// Created by tatiana on 10.01.19.
//

#ifndef ZERLIGHT_CREATEWORLDOPTIONS_H
#define ZERLIGHT_CREATEWORLDOPTIONS_H

#include <string>

namespace game {
    struct CreateWorldOptions {
        CreateWorldOptions(size_t aSizeX=128,size_t aSizeY=128,size_t aSizeZ=128);

        std::string KingdomName;
        size_t SizeX, SizeY, SizeZ;
        unsigned int Seed;

        size_t OffsetX,OffsetY;

        double GroundHeight;
        double GroundFrequency;
        int GroundOctave;
    };
}

#endif //ZERLIGHT_CREATEWORLDOPTIONS_H
