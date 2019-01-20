//
// Created by tatiana on 26.03.2018.
//

#ifndef ZERL_SQUAD_H
#define ZERL_SQUAD_H

#include <memory>
#include <vector>

namespace game
{
    class Character;
    class Formation;
    class SquadPosition;
    class MilitaryStation;
}

namespace game {
    class Squad {
    public:
        Squad();
        ~Squad();

        std::shared_ptr<SquadPosition> position(const Character* member);
        float formationMoveSpeedBonus() const;

        std::shared_ptr<Formation> formation() const;

        bool isAttackTarget(std::shared_ptr<const Character> target) const;

        std::shared_ptr<const Character> squadLeader() const;

        std::shared_ptr<Character> squadLeaderTarget() const;

        std::vector<std::shared_ptr<Character>> members() const;
        bool isInMembers(std::shared_ptr<const Character> candidate) const;

        float commanderDamageBonus(std::shared_ptr<const Character> candidate) const;
        float formationMeleeDamageBonus() const;
        float formationRangedDamageBonus() const;
        float formationAttackSkillBonus() const;
        float formationBlockChanceBonus() const;
        float formationBlockRateBonus() const;
        float formationVisionBonus() const;

        bool dontDefendTarget(std::shared_ptr<const Character> target) const;
        void removeDefendTarget(std::shared_ptr<Character> target);

        void addAttackTarget(std::shared_ptr<Character> target);
        void removeAttackTarget(std::shared_ptr<Character> target);

        std::shared_ptr<MilitaryStation> station() const;
    private:
        std::vector<std::shared_ptr<Character>> mMembers;
        std::shared_ptr<Formation> mFormation;
        std::vector<std::shared_ptr<Character>> mAttackTargets;
        std::vector<std::shared_ptr<Character>> mDontDefendTargets;

        size_t positionIndex(const Character* member);
        std::shared_ptr<SquadPosition> positionByIndex(const size_t& posIndex);

        std::vector<std::shared_ptr<MilitaryStation>> mStations;
    };
}

#endif //ZERL_SQUAD_H
