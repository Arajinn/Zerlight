//
// Created by tatiana on 17.01.19.
//

#include "Envoy.h"
#include "defines.h"
#include "game/map/Region.h"

#include <algorithm>

namespace game
{
    Envoy::Envoy(const float& time,EnvoyState state)
    {
        mDeathCount=0;
        mState=state;
        mChangeStateTime=time;
    }

    Envoy::~Envoy()
    {

    }

    bool Envoy::memberDied(const unsigned int& id)
    {
        const auto iter=std::find_if(mMemberIDs.begin(),mMemberIDs.end(),[&id](unsigned int const& elem)
        {
            return elem==id;
        });

        if (iter==mMemberIDs.end())
            return false;

        mMemberIDs.erase(iter);

        mDeathCount++;

        return true;
    }

    void Envoy::returning(const float& distance)
    {
        if ((!mMemberIDs.empty()) || (mState!=EnvoyState::OnLocation))
            return;

        mState=EnvoyState::Returning;
        mChangeStateTime=distance+REGION->totalTime();
    }
}