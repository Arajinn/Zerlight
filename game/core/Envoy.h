//
// Created by tatiana on 17.01.19.
//

#ifndef ZERLIGHT_ENVOY_H
#define ZERLIGHT_ENVOY_H

#include "game/core/enums.h"
#include <vector>

namespace game {
    class Envoy {
    public:
        Envoy(const float& time,EnvoyState state=EnvoyState::Departing);
        ~Envoy();

        bool memberDied(const unsigned int& id);

        void returning(const float& distance);
    private:
        std::vector<unsigned int> mMemberIDs;
        unsigned int mDeathCount;
        EnvoyState mState;
        float mChangeStateTime;
    };
}

#endif //ZERLIGHT_ENVOY_H
