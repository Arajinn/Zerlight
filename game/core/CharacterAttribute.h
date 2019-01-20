//
// Created by tatiana on 26.03.2018.
//

#ifndef ZERL_CHARACTERATTRIBUTE_H
#define ZERL_CHARACTERATTRIBUTE_H

namespace game {
    class CharacterAttribute
    {
    public:
        CharacterAttribute();
        CharacterAttribute(int AttributeLevel);

        bool increase(const float& xp);

        int attributeLevel() const;
        int startingAttributeLevel() const;
    private:
        int mAttributeLevel;
        int mStartingAttributeLevel;
        float mExperience;
    };
}

#endif //ZERL_CHARACTERATTRIBUTE_H
