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
    struct AttackDef;
    struct FactionDef;
}

namespace game
{
    class Pathfinder;
    class Body;
    class BodySection;
    class Mind;
    class Squad;
    class SquadPosition;
    class Job;
    class CharacterHistory;
    class Item;
    class Weapon;
    class Attack;
    class BodyPartItem;
}

namespace behavior
{
    class Node;
}

namespace game {
class Character : public GameEntity{
    public:
        Character(const map::vector3& position);
        virtual ~Character();

        static std::shared_ptr<Character> create(const map::vector3& position,
                std::shared_ptr<const properties::RaceDefinition> aRaceDef, std::shared_ptr<const properties::FactionDef> aFactionDef,
                const size_t& factionID);

        virtual void pre_update() override;
        virtual void update(float dt) override;
        virtual void moveTo(map::vector3 new_position, bool stopPathing=false) override;
        virtual void spawn(std::shared_ptr<map::MapCell> mapCell) override;
        void setID(const int& value);

        float formationMoveSpeedBonus() const;
        float attributeLevel(CharacterAttributeType attribute) const;
        std::shared_ptr<SquadPosition> squadPosition() const;

        std::shared_ptr<Pathfinder> pathfinder() const;
        std::shared_ptr<const properties::RaceDefinition> raceDefinition() const;
        std::shared_ptr<CharacterHistory> history() const;

        bool pathTo(map::vector3 position, bool adjacent);

        bool claimed() const;

        void setBehavior(behavior::BehaviorType type);

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
        size_t heldItemsCount() const;
        bool hasItem(std::shared_ptr<Item> item) const;

        std::vector<std::shared_ptr<Item>> equippedItems() const;
        size_t equippedItemsCount() const;

        const bool& isLookingForFood() const;
        void setLookingForFood();

        bool canCarry(std::shared_ptr<Item> item) const;
        bool canCarry(EquipmentType equipType) const;
        bool sheathItem();

        bool dropItem(EquipmentType equipSlot);
        bool dropItem(std::shared_ptr<Item> item);

        void eatFoot(std::shared_ptr<Item> item);

        bool isHungry() const;
        bool isStarving() const;

        virtual bool isUpdatable() const;

        virtual std::vector<properties::TileDef> get_draw_info(const int& x, const int& y, const int& z,
                                                 const view_orientation& orientation) const;

        GenderType gender() const;

        bool isSuffocating() const;

        bool findDryLand();

        std::shared_ptr<Character> currentTarget() const;
        bool shouldRunFromTarget() const;
        bool shouldRunFromTarget(std::shared_ptr<const Character> target) const;
        bool shouldStopRunning() const;
        bool runAwayFromTarget();
        bool withinTargetRange() const;
        bool shouldMoveToMaxRange() const;
        bool moveToMaxRange();
        void attackTarget(const float& dt);
        bool defendAttack(const float& attackerWeaponSkill, std::shared_ptr<Attack> attack, std::shared_ptr<BodySection> sectionHit,
                std::vector<std::string>& log_string);

        std::shared_ptr<const Squad> squad() const;

        std::shared_ptr<const properties::FactionDef> factionDef() const;

        bool canPickupWeapon() const;

        std::vector<std::shared_ptr<Weapon>> naturalWeapons() const;

        int skillLevel(const std::string& id) const;

        bool hasAmmo(std::shared_ptr<const Weapon> weapon) const;

        float combatValue() const;

        std::shared_ptr<Body> body() const;
        std::shared_ptr<Mind> mind() const;

        bool hasUniqueName() const;
        std::string name() const;

        float formationBlockChanceBonus() const;
        float formationBlockRateBonus() const;

        void setLastAttacker(std::shared_ptr<CharacterHistory> lastAttacker);

        void lostLimb(std::shared_ptr<BodySection> section);

        map::vector3 lastEnemyLocation() const;

        void setTrackEnemy(const bool& value);
        bool trackEnemy() const;

        std::vector<std::string> templateMaterialIDs() const;

        void removingAttackTarget(std::shared_ptr<Character> target);
    private:
        void init(std::shared_ptr<const properties::RaceDefinition> aRaceDef, std::shared_ptr<const properties::FactionDef> aFactionDef,
                  const size_t& factionID);

        std::shared_ptr<CharacterHistory> mHistory;

        std::shared_ptr<Pathfinder> mPathfinder;
        std::shared_ptr<Body> mBody;
        std::shared_ptr<Mind> mMind;
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

        std::shared_ptr<Character> mCurrentTarget;
        std::shared_ptr<CharacterHistory> mLastAttacker;
        map::vector3 mLastEnemyLocation;
        float mRecentlyRanTimer;
        bool mTrackEnemy;

        bool findOwnedConstruction(ItemEffectType type);
        bool findOwnedPossession(ItemEffectType type);

        bool tryToUseItem(std::shared_ptr<Item> item);

        bool canReach(const map::vector3& pos, bool adjacent) const;

        void abandonNeedGoal();

        void pickingUpItem(std::shared_ptr<Item> item);

        bool updateBody(float dt);

        void actuallyDrop(std::shared_ptr<Item> item);

        bool isAttackTarget(std::shared_ptr<const Character> target) const;

        std::shared_ptr<Character> squadLeaderTarget() const;

        int combatSightRadius() const;

        bool isAfraid(std::shared_ptr<const Character> target) const;

        bool isRetreatingToHeal() const;

        bool isRetreatingToEquip() const;

        bool isRetreatingToRefillAmmo() const;

        float adjustDamageByPerks(std::shared_ptr<const Weapon> weapon, std::shared_ptr<const properties::AttackDef> attackDef) const;

        bool isSneaking() const;

        void increaseAttribute(const CharacterAttributeType& type, const float& value);
        void increaseSquadmatesAttribute(const CharacterAttributeType& type, const float& value);

        void increaseSkill(const std::string& id, const float& value);
        void increaseSquadmatesSkill(const std::string& id, const float& value, bool increaseEquippedWeapon=false);

        float attackMissRate(const std::string& weaponSkillID) const;

        void finishDroppingItems(std::shared_ptr<Character> character);

        void killed();

        std::vector<std::pair<float,std::shared_ptr<map::MapCell>>> mVisibleCells;
        void clearVisibleList();
        void castLight(const int& radius);

        float formationVisionBonus() const;
        int castLightRadius() const;
        void checkSurroundings(const float& dt);

        std::shared_ptr<Character> checkEnemyLOS() const;

        bool isOffMap() const;

        float minimumVisibleDistance() const;

        bool hasLOS(const map::vector3& start,const map::vector3& end) const;

        bool mHasBeenSpotted;
        void spotted();

        bool canTarget(std::shared_ptr<Character> target) const;

        void enterCombat(std::shared_ptr<Character> target);
        void leaveCombat();

        bool shouldTrackEnemy(std::shared_ptr<Character> target) const;

    };
}

#endif //ZERL_CHARACTER_H
