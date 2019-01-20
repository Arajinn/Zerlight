//
// Created by tatiana on 23.12.18.
//

#ifndef ZERLIGHT_WEAPON_H
#define ZERLIGHT_WEAPON_H

#include "game/core/enums.h"

#include <memory>
#include <vector>

namespace properties
{
    struct WeaponDef;
    struct AttackDef;
    struct AmmoDef;
    struct DefendDef;
    struct NaturalWeaponDef;
}

namespace game {
    class Character;
    class BodySection;
    class Item;

    class Weapon : public std::enable_shared_from_this<Weapon>{
    public:
        Weapon(std::shared_ptr<const properties::NaturalWeaponDef> naturalWeapon);
        Weapon(std::shared_ptr<Item> item);
        ~Weapon();

        std::shared_ptr<const properties::WeaponDef> weaponDef() const;

        bool withinRange(const float& distanceSquared, std::shared_ptr<const Character> owner) const;
        float maxRange(std::shared_ptr<const Character> owner) const;

        bool canAttack() const;

        void delayAttack(const float& delay);

        std::shared_ptr<properties::AttackDef> bestAttackInRange(float distance,std::shared_ptr<const Character> owner,
                                                                 std::shared_ptr<const BodySection> sectionHit) const;
        std::shared_ptr<properties::AttackDef> randomAttackInRange(float distance,std::shared_ptr<const Character> owner) const;

        std::shared_ptr<Item> item() const;

        float calculateProjectileDamage(std::shared_ptr<const properties::AmmoDef> ammoDef, const DamageType& damageType) const;
        float calculatePhysicalDamage(const DamageType& damageType, const float& velocity) const;

        float qualityModifier() const;

        void attack(std::shared_ptr<const properties::AttackDef> attackDef);

        std::string name() const;

        bool canDefend() const;
        void defend(std::shared_ptr<const properties::DefendDef> defendDef, float modifier=1.0f);

        void update(const float& dt);
    private:
        std::shared_ptr<const properties::WeaponDef> mWeaponDef;

        float mAttackTimer;
        float mDefendTimer;

        std::shared_ptr<Item> mItem;

        std::string mMaterialID;
        float mSize;

        std::string mName;

        std::shared_ptr<properties::AttackDef> weightedAttackMove(std::vector<std::shared_ptr<properties::AttackDef>> attackMoves) const;
    };
}

#endif //ZERLIGHT_WEAPON_H
