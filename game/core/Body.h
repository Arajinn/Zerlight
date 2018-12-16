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

        float abilityLevel(BodyFunction bodyFunction);

        void addSection(std::shared_ptr<BodySection> section);

        void drinkItem(std::shared_ptr<Item> item);

        bool dropItem(std::shared_ptr<Item> item);
        bool dropItem(std::shared_ptr<Item> item, std::vector<std::shared_ptr<BodySection>> sections);
        std::vector<std::shared_ptr<Item>> getDropedItem(EquipmentType equipType);

        bool isStarving() const;
        bool isHungry() const;

        std::vector<std::shared_ptr<Item>> heldItems() const;
        bool hasItem(std::shared_ptr<Item> item) const;

        const bool& isLookingForFood() const;
        void setLookingForFood();

        bool canCarry(std::vector<std::shared_ptr<BodySection>> sectionList) const;
        bool canCarry(EquipmentType equipType) const;
        bool canCarry(std::shared_ptr<Item> item) const;

        bool sheathItem();

        void eatFoot(std::shared_ptr<Item> item);

        std::vector<properties::TileDef> getTiles() const;

        GenderType gender() const;
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

        std::vector<HealthStatusEffect> mStatusEffects;
        bool containStatusEffect(HealthStatusAilment key) const;

        std::vector<BodySectionInfo> mEquipmentSlots;
        bool isEqupmentSlotsContain(EquipmentType equipType) const;

        std::shared_ptr<BodySection> mMainBodySection;
        std::vector<std::shared_ptr<BodySection>> mBodySections;

        std::vector<int> mBodyFunctionTotal, mBodyFunctionCurrent;

        std::vector<std::shared_ptr<Item>> mHeldItems;
        std::vector<std::shared_ptr<Item>> mEquippedItems;

        void giveBirth();

        std::vector<EquipItemInfo> mAdditionalEquipmentSlots;
    };
}

#endif //ZERL_BODY_H
