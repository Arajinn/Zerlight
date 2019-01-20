//
// Created by tatiana on 10.01.19.
//

#include "Faction.h"
#include "defines.h"
#include "AIDirector.h"
#include "Faction.h"
#include "Fortress.h"
#include "Military.h"
#include "Character.h"
#include "CharacterHistory.h"
#include "Ambassador.h"
#include "Envoy.h"
#include "game/properties/FactionDef.h"

#include <algorithm>

namespace game
{
    Faction::Faction(std::shared_ptr<const properties::FactionDef> factionDef, const std::string& name,
            const size_t& factionGUID, float distance)
    {
        mFactionGUID=factionGUID;
        mFactionDef=factionDef;
        mName=name;
        mHighestPopulation=0;
        mDistance=distance;
    }

    Faction::~Faction()
    {
        mFactionDef=nullptr;
    }

    std::shared_ptr<const properties::FactionDef> Faction::factionDef() const
    {
        return mFactionDef;
    }

    void Faction::setFactionRelation(const size_t& factionID, const float& relation)
    {
        auto iter=std::find_if(mFactionRelations.begin(),mFactionRelations.end(),[&factionID](std::pair<int,float> const& elem)
        {
            return elem.first==factionID;
        });

        if (iter==mFactionRelations.end())
            mFactionRelations.push_back({factionID,relation});
        else
            iter->second=relation;
    }

    float Faction::factionRelation(const size_t& factionID) const
    {
        auto iter=std::find_if(mFactionRelations.begin(),mFactionRelations.end(),[&factionID](std::pair<size_t,float> const& elem)
        {
            return elem.first==factionID;
        });

        if (iter==mFactionRelations.end())
            return 0.0f;
        else
            return iter->second;
    }

    bool Faction::isHostile(const size_t& factionID) const
    {
        if (mFactionDef->Type==FactionType::Wild)
            return true;

        if (mFactionGUID==factionID)
            return false;

        if (AIDIRECTOR->faction(factionID)->factionDef()->Type==FactionType::Wild)
            return true;

        return (factionRelation(factionID)<0.0f);
    }

    size_t Faction::id() const
    {
        return mFactionGUID;
    }

    void Faction::addMember(std::shared_ptr<Character> character)
    {
        auto iter=std::find_if(mMembers.begin(),mMembers.end(),[&character](std::shared_ptr<Character> const& elem)
        {
            return elem->ID()==character->ID();
        });

        if (iter!=mMembers.end())
            return;

        mMembers.push_back(character);

        if (mMembers.size() <= mHighestPopulation)
            return;

        mHighestPopulation=mMembers.size();
    }

    void Faction::removeMember(std::shared_ptr<Character> character)
    {
        auto iter=std::find_if(mMembers.begin(),mMembers.end(),[&character](std::shared_ptr<Character> const& elem)
        {
            return elem->ID()==character->ID();
        });

        if (iter!=mMembers.end())
            mMembers.erase(iter);
    }

    std::vector<std::shared_ptr<Character>> Faction::members() const
    {
        return mMembers;
    }

    bool Faction::isDeceasedMembersContain(const unsigned int& characterID) const
    {
        const auto iter=std::find_if(mDeceasedMembers.begin(),mDeceasedMembers.end(),
                [&characterID](std::shared_ptr<CharacterHistory> const& elem)
                {
                    return (elem->characterID()==characterID);
                });

        return (iter!=mDeceasedMembers.end());
    }

    void Faction::memberDied(std::shared_ptr<Character> character)
    {
        FORTRESS->military()->removeAttackTarget(character);
        removeMember(character);

        auto iter=std::find_if(mDeceasedMembers.begin(),mDeceasedMembers.end(),[&character](std::shared_ptr<CharacterHistory> const& elem)
        {
            return elem->characterID()==character->ID();
        });

        if (iter==mDeceasedMembers.end())
            mDeceasedMembers.push_back(character->history());

        character->history()->died();

        if (mAmbassador->ID()==character->ID())
        {
            mAmbassador=nullptr;
            std::cout << "The ambassador of "+mName+" has died." << std::endl;
        }

        for (const auto& envoy : mEnvoys)
        {
            if (envoy->memberDied(character->ID()))
            {
                envoy->returning(mDistance);
                return;
            }
        }
    }
}