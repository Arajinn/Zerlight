//
// Created by tatiana on 23.12.18.
//

#include "CharacterSkill.h"
#include "game/utils/Randomizer.h"

namespace game {
    CharacterSkill::CharacterSkill()
    {
        mStartingSkillLevel=setStartingSkillLevel(1,20,10);
        mSkillLevel=mStartingSkillLevel;
        mExperience=0.0f;
    }

    CharacterSkill::~CharacterSkill()
    {

    }

    int CharacterSkill::setStartingSkillLevel(int min, int max, int mean)
    {
        return int(RANDOMIZER->norm(min,max,mean));
    }

    const int& CharacterSkill::skillLevel() const
    {
        return mSkillLevel;
    }

    bool CharacterSkill::increase(const float& value)
    {
        mExperience+=value;

        const float threshold=10.0f+float(mSkillLevel-mStartingSkillLevel)*5.0f;
        if (mExperience<threshold)
            return false;

        mSkillLevel++;
        mExperience-=threshold;
        return true;
    }
}