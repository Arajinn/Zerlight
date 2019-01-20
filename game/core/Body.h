//
// Created by tatiana on 26.03.2018.
//

#ifndef ZERL_BODY_H
#define ZERL_BODY_H

#include "game/core/enums.h"
#include "game/properties/TileDef.h"

#include <vector>
#include <memory>

namespace game
{
    class Character;
    class HealthStatusEffect;
    class Item;
    class BodySection;
    class BodyPart;
    class Weapon;
    class Attack;
}

namespace game {
    struct BodySectionInfo
    {
    public:
        EquipmentType type;
        std::vector<std::shared_ptr<BodySection>> sections;
    };

    struct EquipItemInfo
    {
    public:
        EquipmentType type;
        std::shared_ptr<Item> item;
    };

    class Body : public std::enable_shared_from_this<Body> {
    public:
        Body();
        ~Body();

        static std::shared_ptr<Body> create(std::shared_ptr<Character> parent);

        bool update(float dt);

        float moveSpeed() const;
        float exhaustionModifier() const;
        float movementPenalty() const;

        const bool& isDead() const;

        bool isThirsty() const;
        bool isDyingOfThirst() const;

        const bool& isLookingForDrink() const;
        void setLookingForDrink();

        bool pickupItem(std::shared_ptr<Item> item);
        bool pickupItem(std::shared_ptr<Item> item, EquipmentType equipType);
        bool pickupItem(std::shared_ptr<Item> item, std::vector<std::shared_ptr<BodySection>> sections);
        void addEquipmentSlot(std::shared_ptr<BodySection> section, EquipmentType equipType);
        void addEquipmentSlot(std::shared_ptr<BodySection> section);

        void addFunction(std::shared_ptr<BodyPart> bodyPart);
        void regainFunction(std::shared_ptr<BodyPart> bodyPart);
        void lostFunction(std::shared_ptr<BodyPart> bodyPart);
        void completelyLostFunction(std::shared_ptr<BodyPart> bodyPart);
        void damageFunction(std::shared_ptr<BodyPart> bodyPart);

        float abilityLevel(BodyFunction bodyFunction);

        void addSection(std::shared_ptr<BodySection> section);

        void drinkItem(std::shared_ptr<Item> item);

        bool dropItem(std::shared_ptr<Item> item);
        bool dropItem(std::shared_ptr<Item> item, std::vector<std::shared_ptr<BodySection>> sections);
        std::vector<std::shared_ptr<Item>> getDropedItem(EquipmentType equipType);

        bool isStarving() const;
        bool isHungry() const;

        std::vector<std::shared_ptr<Item>> heldItems() const;
        size_t heldItemsCount() const;
        bool hasItem(std::shared_ptr<Item> item) const;

        std::vector<std::shared_ptr<Item>> equippedItems() const;
        size_t equippedItemsCount() const;

        const bool& isLookingForFood() const;
        void setLookingForFood();

        bool canCarry(std::vector<std::shared_ptr<BodySection>> sectionList) const;
        bool canCarry(EquipmentType equipType) const;
        bool canCarry(std::shared_ptr<Item> item) const;
        bool canCarry() const;

        bool sheathItem();

        void eatFoot(std::shared_ptr<Item> item);

        std::vector<properties::TileDef> getTiles() const;

        GenderType gender() const;

        bool isSuffocating() const;

        const float& combatValue() const;

        bool canPickupWeapon() const;

        std::vector<std::shared_ptr<Weapon>> naturalWeapons() const;

        bool hasWeaponInRange(float distance) const;

        bool hasAmmo(std::shared_ptr<const Weapon> weapon) const;

        float maxRange() const;

        bool isBleeding() const;

        std::vector<std::shared_ptr<Weapon>> weapons(float distance) const;
        std::vector<std::shared_ptr<Weapon>> weapons(bool rangeCheck, float distance=0.0f) const;

        std::shared_ptr<BodySection> randomSection(std::shared_ptr<const Character> attacker = nullptr) const;

        bool containStatusEffect(const HealthStatusAilment& key) const;
        void removeStatusEffect(const HealthStatusAilment& key);
        void addStatusEffect(const HealthStatusAilment& key, const float& amount, const float& recoveryRate, const bool& effectRecoveryRate);

        std::vector<std::shared_ptr<Item>> equippedItemsInSlot(EquipmentType equipType) const;
        int equippedItemsInSlotCount(EquipmentType equipType) const;

        std::shared_ptr<Item> consumeAmmo(const std::string& ammoType, bool deleteAmmo=true) const;

        bool isSleeping() const;

        std::vector<std::shared_ptr<Weapon>> defenseWeapons() const;

        bool canDodge() const;
        void dodged();

        void damage(std::shared_ptr<Attack> attack, std::shared_ptr<BodySection> sectionHit, std::vector<std::string>& hitEffects,
                    std::vector<std::string>& statusEffects);

        std::shared_ptr<Character> character() const;

        void addNaturalWeapon(std::shared_ptr<Weapon> weapon);
        void removeNaturalWeapon(std::shared_ptr<Weapon> weapon);

        bool unequip(std::shared_ptr<Item> item);
        bool unequipAdditional(std::shared_ptr<Item> item);

        bool remove(std::shared_ptr<Item> item);
        void removeArtificialLimb(std::shared_ptr<Item> item);

        std::vector<std::shared_ptr<Item>> itemsToDrop() const;
        size_t itemsToDropCount() const;

        void repairEquipment();

        std::vector<std::shared_ptr<BodySection>> bodySections() const;
    private:
        void init(std::shared_ptr<Character> parent);

        std::shared_ptr<Character> owner;

        unsigned int bodySectionsKey;

        float mMovementPenalty;
        float mJobPenalty;

        bool mIsDead;

        float mBloodLevel;
        float mBloodLossRate;
        float mSpawnBloodTimer;

        bool mIsSleeping;
        float mRestLevel;
        float mRestRate;

        bool mIsBleeding;

        float mThirstLevel;
        bool mLookingForDrink;

        float mHungerLevel;
        bool mLookingForFood;

        float mGestationTime;
        bool mReadyToMate;

        float mCombatValue;

        float mLungCapacity;

        float mPickupWeaponTimer;

        std::vector<HealthStatusEffect> mStatusEffects;

        std::vector<BodySectionInfo> mEquipmentSlots;
        bool isEqupmentSlotsContain(EquipmentType equipType) const;
        std::vector<std::shared_ptr<BodySection>> getSectionsByEqupSlot(const EquipmentType& equipType) const;

        std::shared_ptr<BodySection> mMainBodySection;
        std::vector<std::shared_ptr<BodySection>> mBodySections;

        std::vector<int> mBodyFunctionTotal, mBodyFunctionCurrent;
        std::vector<bool> mDamagedFunction, mLostFunction;

        std::vector<std::shared_ptr<Item>> mHeldItems;
        std::vector<std::shared_ptr<Item>> mEquippedItems;

        void giveBirth();

        std::vector<EquipItemInfo> mAdditionalEquipmentSlots;

        std::vector<std::shared_ptr<Weapon>> mNaturalWeapons;
        std::vector<std::shared_ptr<Weapon>> mHeldWeapons;

        std::vector<std::shared_ptr<Item>> additionalEquippedItemsInSlot(EquipmentType equipType) const;

        float adjustHitWeightByPerk(std::shared_ptr<const Character> attacker, SquadPositionPerk perk,
                std::shared_ptr<const BodySection> section) const;

        float mDodgeTimer;

        void wakeUp();

        std::vector<std::shared_ptr<Item>> mItemsToDrop;

        float combinedBloodLossRate() const;
        void combineStatusEffects(const std::vector<BodySectionStatus>& oldStatus,std::shared_ptr<BodySection> section,
                std::vector<std::string>& statusEffects) const;
        void combineDamageFunctions(std::vector<std::string>& statusEffects);
        void combineLostFunctions(std::vector<std::string>& statusEffects);
        void combineAilments(const std::vector<HealthStatusAilment>& oldStatus, std::vector<std::string>& statusEffects) const;
    };
}

#endif //ZERL_BODY_H
