//
// Created by tatiana on 26.03.2018.
//

#include "defines.h"
#include "Squad.h"
#include "Formation.h"
#include "Character.h"
#include "SquadPosition.h"
#include "Body.h"
#include "Item.h"
#include "MilitaryStation.h"
#include "game/properties/UniformSettings.h"

#include <limits>
#include <algorithm>

namespace game
{
    Squad::Squad()
            :mFormation(nullptr)
    {
        mStations.resize(3);
        for (size_t i=0,isize=mStations.size();i<isize;i++)
            mStations[i]=nullptr;
    }

    Squad::~Squad()
    {

    }

    size_t Squad::positionIndex(const Character* member)
    {
        if (member==nullptr)
            return std::numeric_limits<size_t>::max();

        for (size_t i=0,isize=mMembers.size();i<isize;i++)
        {
            if (mMembers.at(i).get()==member)
                return i;
        }

        return std::numeric_limits<size_t>::max();
    }

    std::shared_ptr<SquadPosition> Squad::positionByIndex(const size_t& posIndex)
    {
        if (mFormation== nullptr)
            return nullptr;

        if (posIndex>=mFormation->positions().size())
            return nullptr;

        return mFormation->positions().at(posIndex);
    }

    std::shared_ptr<SquadPosition> Squad::position(const Character* member)
    {
        return positionByIndex(positionIndex(member));
    }

    float Squad::formationMoveSpeedBonus() const
    {
        return 1.0f;
    }

    std::shared_ptr<Formation> Squad::formation() const
    {
        return mFormation;
    }

    bool Squad::isAttackTarget(std::shared_ptr<const Character> target) const
    {
        auto iter=std::find_if(mAttackTargets.begin(),mAttackTargets.end(),[&target](std::shared_ptr<Character> const& item)
        {
            return item->ID()==target->ID();
        });

        return (iter!=mAttackTargets.end());
    }

    std::shared_ptr<const Character> Squad::squadLeader() const
    {
        if (!mMembers.empty())
            return mMembers.at(0);

        return nullptr;
    }

    std::shared_ptr<Character> Squad::squadLeaderTarget() const
    {
        const auto leader=squadLeader();
        if (leader==nullptr)
            return nullptr;

        return leader->currentTarget();
    }

    std::vector<std::shared_ptr<Character>> Squad::members() const
    {
        return mMembers;
    }

    bool Squad::isInMembers(std::shared_ptr<const Character> candidate) const
    {
        auto iter=std::find_if(mMembers.begin(),mMembers.end(),[&candidate](std::shared_ptr<Character> const& member)
        {
            return member->ID()==candidate->ID();
        });

        return (iter!=mMembers.end());
    }

    float Squad::commanderDamageBonus(std::shared_ptr<const Character> candidate) const
    {
        for (const auto& member : mMembers)
        {
            if (member==nullptr)
                continue;

            if (member->ID()==candidate->ID())
                continue;

            if (member->currentTarget()->ID()!=candidate->currentTarget()->ID())
                continue;

            const auto squad_pos=member->squadPosition();

            if (squad_pos==nullptr)
                continue;

            if (squad_pos->perk()!=SquadPositionPerk::Commander)
                continue;

            if (member->body()->equippedItemsInSlotCount(EquipmentType::Head)!=0)
                continue;

            return 1.1f;
        }

        return 1.0f;
    }

    float Squad::formationMeleeDamageBonus() const
    {
        if (mFormation==nullptr)
            return 1.0f;

        if (mFormation->perk()!=FormationPerk::BestDefense)
            return 1.0f;

        float summ=0.0f;
        for (const auto& member : mMembers)
        {
            if (member==nullptr)
                continue;

            if (member->body()->maxRange()<=2.0f)
                summ+=2.0f;
        }

        return (1.0f+summ*0.00999999977648258f);
    }

    float Squad::formationRangedDamageBonus() const
    {
        if (mFormation==nullptr)
            return 1.0f;

        if (mFormation->perk()!=FormationPerk::FiringSquad)
            return 1.0f;

        float summ=0.0f;
        for (const auto& member : mMembers)
        {
            if (member==nullptr)
                continue;

            if (member->body()->maxRange()>2.0f)
                summ+=2.0f;
        }

        return (1.0f+summ*0.00999999977648258f);
    }

    float Squad::formationAttackSkillBonus() const
    {
        if (mFormation==nullptr)
            return 1.0f;

        if (mFormation->perk()!=FormationPerk::SuppressiveFire)
            return 1.0f;

        float summ=0.0;
        for (const auto& member : mMembers)
        {
            if (member==nullptr)
                continue;

            if (member->body()->maxRange()<=2.0f)
                continue;

            summ+=8.33333f;
        }

        return (1.0f+summ*0.00999999977648258f);
    }

    float Squad::formationBlockChanceBonus() const
    {
        if (mFormation==nullptr)
            return 1.0f;

        if (mFormation->perk()!=FormationPerk::ShieldWall)
            return 1.0f;

        float summ=0.0f;
        for (const auto& member : mMembers)
        {
            if (member!=nullptr)
            {
                const auto heldItems=member->body()->heldItems();
                for (const auto& item : heldItems)
                {
                    if (UNIFORM_SETTINGS->isShield(item->itemID()))
                    {
                        summ+=2.0f;
                        break;
                    }
                }
            }
        }

        return (1.0f+summ*0.00999999977648258f);
    }

    float Squad::formationBlockRateBonus() const
    {
        if (mFormation==nullptr)
            return 1.0f;

        if (mFormation->perk()!=FormationPerk::ShieldWall)
            return 1.0f;

        float summ=0.0f;
        for (const auto& member : mMembers)
        {
            if (member!=nullptr)
            {
                const auto heldItems=member->body()->heldItems();
                for (const auto& item : heldItems)
                {
                    if (UNIFORM_SETTINGS->isShield(item->itemID()))
                    {
                        summ+=2.0f;
                        break;
                    }
                }
            }
        }

        return (1.0f-summ*0.00999999977648258f);
    }

    float Squad::formationVisionBonus() const
    {
        if (mFormation==nullptr)
            return 1.0f;

        if (mFormation->perk()!=FormationPerk::KeepYourEyesOpen)
            return 1.0f;

        float summ=0.0f;
        for (const auto& member : mMembers)
        {
            if (member!= nullptr)
                summ+=5.0f;
        }

        return (1.0f+summ*0.00999999977648258f);
    }

    bool Squad::dontDefendTarget(std::shared_ptr<const Character> target) const
    {
        const auto iter=std::find_if(mDontDefendTargets.begin(),mDontDefendTargets.end(),
                [&target](std::shared_ptr<Character> const& elem)
                {
                    return elem->ID()==target->ID();
                });

        return (iter!=mDontDefendTargets.end());
    }

    void Squad::addAttackTarget(std::shared_ptr<Character> target)
    {
        const auto iter1=std::find_if(mAttackTargets.begin(),mAttackTargets.end(),
                                     [&target](std::shared_ptr<Character> const& elem)
                                     {
                                         return elem->ID()==target->ID();
                                     });

        if (iter1!=mAttackTargets.end())
            return;

        mAttackTargets.push_back(target);

        const auto iter2=std::find_if(mDontDefendTargets.begin(),mDontDefendTargets.end(),
                                     [&target](std::shared_ptr<Character> const& elem)
                                     {
                                         return elem->ID()==target->ID();
                                     });

        if (iter2!=mDontDefendTargets.end())
            mDontDefendTargets.erase(iter2);
    }

    void Squad::removeAttackTarget(std::shared_ptr<Character> target)
    {
        const auto iter1=std::find_if(mAttackTargets.begin(),mAttackTargets.end(),
                                      [&target](std::shared_ptr<Character> const& elem)
                                      {
                                          return elem->ID()==target->ID();
                                      });

        if (iter1==mAttackTargets.end())
            return;

        mAttackTargets.erase(iter1);

        const auto iter2=std::find_if(mDontDefendTargets.begin(),mDontDefendTargets.end(),
                                      [&target](std::shared_ptr<Character> const& elem)
                                      {
                                          return elem->ID()==target->ID();
                                      });

        if (iter2==mDontDefendTargets.end())
            mDontDefendTargets.push_back(target);

        for (const auto& member : mMembers)
            member->removingAttackTarget(target);
    }

    void Squad::removeDefendTarget(std::shared_ptr<Character> target)
    {
        const auto iter=std::find_if(mDontDefendTargets.begin(),mDontDefendTargets.end(),
                                      [&target](std::shared_ptr<Character> const& elem)
                                      {
                                          return elem->ID()==target->ID();
                                      });

        if (iter!=mDontDefendTargets.end())
            mDontDefendTargets.erase(iter);
    }

    std::shared_ptr<MilitaryStation> Squad::station() const
    {
        return mStations.at(MilitaryStation::currentShift());
    }
}