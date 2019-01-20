//
// Created by tatiana on 23.12.18.
//

#ifndef ZERLIGHT_CHARACTERSKILL_H
#define ZERLIGHT_CHARACTERSKILL_H

namespace game {
    class CharacterSkill {
    public:
        CharacterSkill();
        ~CharacterSkill();

        const int& skillLevel() const;
        bool increase(const float& value);
    private:
        int mSkillLevel;
        int mStartingSkillLevel;
        float mExperience;

        int setStartingSkillLevel(int min, int max, int mean);
    };
}

#endif //ZERLIGHT_CHARACTERSKILL_H
