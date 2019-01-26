//
// Created by tatiana on 09.06.2018.
//

#ifndef ZERL_CHARACTERHISTORY_H
#define ZERL_CHARACTERHISTORY_H

#include "game/core/enums.h"
#include "game/core/types.h"

#include <string>
#include <memory>
#include <vector>
#include <game/properties/FactionDef.h>

namespace properties
{
    struct RaceDefinition;
    struct RaceClassDef;
}

namespace game
{
    class Character;
    class CharacterSkill;
    struct CharacterAttribute;
}

namespace game {
    class CharacterHistory {
    public:
        CharacterHistory(std::shared_ptr<const properties::RaceDefinition> aRaceDef,
                std::shared_ptr<const properties::FactionDef> aFactionDef, const size_t& factionID);
        ~CharacterHistory();

        void setName(const std::string& value);
        std::string name() const;

        const GenderType& gender() const;
        std::shared_ptr<const properties::FactionDef> factionDef() const;
        size_t factionID() const;

        std::shared_ptr<const properties::RaceClassDef> raceClassDef() const;
        std::shared_ptr<const properties::RaceDefinition> raceDef() const;

        int skillLevel(const std::string& id) const;
        float skillModifier(const std::string& id) const;
        bool increaseSkill(const std::string& id, const float& value);

        float attributeLevel(const CharacterAttributeType& type) const;
        bool increaseAttribute(const CharacterAttributeType& type, const float& value);
        int attributeDeltaLevel(const CharacterAttributeType& type) const;

        bool hasUniqueName() const;

        std::string possessivePronoun() const;
        std::string pronoun() const;

        MaterialID_t materialAtIndex(const int& index) const;
        std::vector<MaterialID_t> templateMaterialIDs() const;

        void killed(std::shared_ptr<Character> victim);

        void died();

        unsigned int characterID() const;
        void setCharaterTD(const unsigned int& characterID);
    private:
        std::shared_ptr<const properties::RaceDefinition> mRaceDef;
        std::shared_ptr<const properties::RaceClassDef> mRaceClassDef;
        std::shared_ptr<const properties::FactionDef> mFactionDef;
        size_t mFactionID;
        unsigned int mCharacterID;

        std::string mName;
        GenderType mGender;
        std::vector<MaterialID_t> mTemplateMaterialIDs;

        std::vector<std::pair<std::string,std::shared_ptr<CharacterSkill>>> mSkills;
        std::vector<std::pair<CharacterAttributeType,std::shared_ptr<CharacterAttribute>>> mAttributes;

        int rawAttributeLevel(const CharacterAttributeType& type) const;

        std::string genderName() const;
        std::string generateName() const;

        std::vector<std::shared_ptr<CharacterHistory>> mKills;

        unsigned int mJoinDate,mDiedDate;
    };
}

#endif //ZERL_CHARACTERHISTORY_H
