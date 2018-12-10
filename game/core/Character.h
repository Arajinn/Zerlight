//
// Created by tatiana on 26.03.2018.
//

#ifndef ZERL_CHARACTER_H
#define ZERL_CHARACTER_H

#include "game/core/GameEntity.h"
#include "game/core/enums.h"
#include "game/core/CharacterAttribute.h"
#include "game/behavior/TaskResult.h"
#include "game/map/vector3.h"

#include <memory>
#include <vector>

namespace properties
{
    struct RaceDefinition;
}

namespace game
{
    class Pathfinder;
    class Body;
    class Mind;
    class Squad;
    class SquadPosition;
    class Job;
    class CharacterHistory;
    class Item;
}

namespace behavior
{
    class Node;
}

namespace game {
class Character : public GameEntity{
    public:
        Character(const map::vector3& position);
        ~Character();

        static std::shared_ptr<Character> create(const map::vector3& position,
                std::shared_ptr<const properties::RaceDefinition> aRaceDef);

        virtual void pre_update() override;
        virtual void update(float dt) override;
        virtual void moveTo(map::vector3 new_position) override;
        virtual void spawn(std::shared_ptr<map::MapCell> mapCell) override;

        float formationMoveSpeedBonus() const;
        float attributeLevel(CharacterAttributeType attribute) const;
        std::shared_ptr<SquadPosition> squadPosition() const;

        std::shared_ptr<Pathfinder> pathfinder() const;
        std::shared_ptr<const properties::RaceDefinition> raceDefinition() const;
        std::shared_ptr<CharacterHistory> history() const;

        bool pathTo(map::vector3 position, bool adjacent);

        bool claimed() const;

        void setBehavior(behavior::BehaviorType type);

        bool isThirsty() const;
        bool isDyingOfThirst() const;

        const bool& isLookingForDrink() const;
        void setLookingForDrink();

        std::shared_ptr<GameEntity> currentNeedGoal() const;

        std::shared_ptr<Job> job() const;
        void cancelJob(std::string reason = "");

        bool findEntity(ItemEffectType type);

        map::vector3 desiredPosition() const;
        bool atEndPosition() const;

        bool pickupItem(std::shared_ptr<Item> item);

        float abilityLevel(BodyFunction bodyFunction);

        void drinkItem(std::shared_ptr<Item> item);

        void setNeedGoal(std::shared_ptr<GameEntity> ent);

        std::vector<std::shared_ptr<Item>> heldItems() const;
        bool hasItem(std::shared_ptr<Item> item) const;

        const bool& isLookingForFood() const;
        void setLookingForFood();

        bool canCarry(std::shared_ptr<Item> item) const;
        bool canCarry(EquipmentType equipType) const;
        bool sheathItem();

        bool dropItem(EquipmentType equipSlot);

        void eatFoot(std::shared_ptr<Item> item);

        bool isHungry() const;
        bool isStarving() const;

        virtual bool isUpdatable() const;
    private:
        friend class Body;
        friend class Mind;
        void init(std::shared_ptr<const properties::RaceDefinition> aRaceDef);

        std::shared_ptr<CharacterHistory> mHistory;

        std::shared_ptr<Pathfinder> mPathfinder;
        std::shared_ptr<Body> body;
        std::shared_ptr<Mind> mind;
        std::shared_ptr<Squad> mSquad;

        std::vector<CharacterAttribute> mAttributes;

        std::shared_ptr<Job> mJob;

        float mMoveTime;
        float mCastLightTimer;

        map::vector3 mStartingFallPosition;

        std::shared_ptr<Character> mClaimedBy;

        behavior::BehaviorType mBehaviorType;
        std::shared_ptr<behavior::Node> mRootBehavior;

        std::shared_ptr<GameEntity> mCurrentNeedGoal;
        std::vector<std::shared_ptr<Item>> mPossessions;

        bool findOwnedConstruction(ItemEffectType type);
        bool findOwnedPossession(ItemEffectType type);

        bool tryToUseItem(std::shared_ptr<Item> item);

        bool canReach(const map::vector3& pos, bool adjacent);

        void abandonNeedGoal();

        void pickingUpItem(std::shared_ptr<Item> item);

        bool updateBody(float dt);

        void actuallyDrop(std::shared_ptr<Item> item);
    };
}

#endif //ZERL_CHARACTER_H
