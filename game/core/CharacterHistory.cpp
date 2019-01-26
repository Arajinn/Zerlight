//
// Created by tatiana on 09.06.2018.
//

#include "CharacterHistory.h"
#include "defines.h"
#include "Character.h"
#include "CharacterSkill.h"
#include "CharacterAttribute.h"
#include "game/properties/RaceDefinition.h"
#include "game/properties/GenderDef.h"
#include "game/properties/RaceClassDef.h"
#include "game/properties/MaterialDef.h"

#include <algorithm>

namespace game
{
    CharacterHistory::CharacterHistory(std::shared_ptr<const properties::RaceDefinition> aRaceDef,
            std::shared_ptr<const properties::FactionDef> aFactionDef, const size_t& factionID)
        :mRaceDef(aRaceDef)
        ,mRaceClassDef(nullptr)
        ,mFactionID(factionID)
        ,mCharacterID(std::numeric_limits<unsigned int>::max())
    {
        mGender=aRaceDef->generateGender();
        mFactionDef=aFactionDef;

        mJoinDate=REGION->day();
        mDiedDate=std::numeric_limits<unsigned int>::max();
    }

    CharacterHistory::~CharacterHistory()
    {

    }

    void CharacterHistory::setCharaterTD(const unsigned int& characterID)
    {
        mCharacterID=characterID;
    }

    void CharacterHistory::setName(const std::string& value)
    {
        mName=value;
    }

    const GenderType& CharacterHistory::gender() const
    {
        return mGender;
    }

    std::shared_ptr<const properties::FactionDef> CharacterHistory::factionDef() const
    {
        return mFactionDef;
    }

    size_t CharacterHistory::factionID() const
    {
        return mFactionID;
    }

    std::shared_ptr<const properties::RaceClassDef> CharacterHistory::raceClassDef() const
    {
        return mRaceClassDef;
    }

    std::shared_ptr<const properties::RaceDefinition> CharacterHistory::raceDef() const
    {
        return mRaceDef;
    }

    int CharacterHistory::skillLevel(const std::string& id) const
    {
        auto iter=std::find_if(mSkills.begin(),mSkills.end(),[&id](std::pair<std::string,std::shared_ptr<CharacterSkill>> const& item)
        {
            return id==item.first;
        });

        if (iter==mSkills.end())
            return 1;
        else
            return iter->second->skillLevel();
    }

    float CharacterHistory::skillModifier(const std::string& id) const
    {
        return float(skillLevel(id))/100.0f;
    }

    bool CharacterHistory::increaseSkill(const std::string& id, const float& value)
    {
        auto iter=std::find_if(mSkills.begin(),mSkills.end(),[&id](std::pair<std::string,std::shared_ptr<CharacterSkill>> const& item)
        {
            return id==item.first;
        });

        if (iter==mSkills.end())
            return false;
        else
            return iter->second->increase(value);
    }

    int CharacterHistory::rawAttributeLevel(const CharacterAttributeType& type) const
    {
        auto iter=std::find_if(mAttributes.begin(),mAttributes.end(),
                               [&type](std::pair<CharacterAttributeType,std::shared_ptr<CharacterAttribute>> const& elem)
                               {
                                   return elem.first==type;
                               });

        if (iter!=mAttributes.end())
            return iter->second->attributeLevel();
        else
            return 100;
    }

    float CharacterHistory::attributeLevel(const CharacterAttributeType& type) const
    {
        return float(rawAttributeLevel(type))/100.0f;
    }

    bool CharacterHistory::increaseAttribute(const CharacterAttributeType& type, const float& value)
    {
        auto iter=std::find_if(mAttributes.begin(),mAttributes.end(),
                [&type](std::pair<CharacterAttributeType,std::shared_ptr<CharacterAttribute>> const& elem)
        {
            return elem.first==type;
        });

        if (iter==mAttributes.end())
            return false;
        else
            return iter->second->increase(value);
    }

    int CharacterHistory::attributeDeltaLevel(const CharacterAttributeType& type) const
    {
        auto iter=std::find_if(mAttributes.begin(),mAttributes.end(),
                               [&type](std::pair<CharacterAttributeType,std::shared_ptr<CharacterAttribute>> const& elem)
                               {
                                   return elem.first==type;
                               });

        if (iter==mAttributes.end())
            return 0;
        else
            return iter->second->attributeLevel()-iter->second->startingAttributeLevel();
    }

    bool CharacterHistory::hasUniqueName() const
    {
        return (!mName.empty());
    }

    std::string CharacterHistory::genderName() const
    {
        if (hasUniqueName())
            return mName;

        auto genderDef=raceDef()->gender(mGender);
        if (genderDef!=nullptr)
            genderDef->Name;
        else
            return raceDef()->Name;
    }

    std::vector<MaterialID_t> CharacterHistory::templateMaterialIDs() const
    {
        if (!mTemplateMaterialIDs.empty())
            return mTemplateMaterialIDs;
        else
            raceClassDef()->TemplateMaterialIDs;
    }

    MaterialID_t CharacterHistory::materialAtIndex(const int& index) const
    {
        if (index<0)
            return "";

        const auto materialIDs=templateMaterialIDs();
        return materialIDs.at((size_t)index);
    }

    std::string CharacterHistory::generateName() const
    {
        std::string result=genderName();

        if (raceClassDef()==nullptr)
            return result;

        const auto templateMaterials=templateMaterialIDs();
        if (templateMaterials.empty())
            return result;

        std::string materialName=GAME_DEFINITIONS->materialDefinition(templateMaterials.front())->GroupNameOrName();
        result=materialName+" "+result;
        return result;
    }

    std::string CharacterHistory::name() const
    {
        if (hasUniqueName())
            return mName;

        return generateName();
    }

    std::string CharacterHistory::possessivePronoun() const
    {
        switch (mGender)
        {
            case GenderType::Neuter:
                return "its";
            case GenderType::Male:
                return "his";
            case GenderType::Female:
                return "her";
            default:
                return "";
        }
    }

    std::string CharacterHistory::pronoun() const
    {
        switch (mGender)
        {
            case GenderType::Neuter:
                return "it";
            case GenderType::Male:
                return "he";
            case GenderType::Female:
                return "she";
            default:
                return "";
        }
    }

    void CharacterHistory::died()
    {
        mDiedDate=REGION->day();
    }

    unsigned int CharacterHistory::characterID() const
    {
        return mCharacterID;
    }

    void CharacterHistory::killed(std::shared_ptr<Character> victim)
    {
        mKills.push_back(victim->history());

        if (hasUniqueName())
            return;

        if (mKills.size()<10)
            return;

        // TODO
    }
}