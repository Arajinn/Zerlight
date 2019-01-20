//
// Created by tatiana on 6/14/2018.
//

#ifndef ZERL_BODYPART_H
#define ZERL_BODYPART_H

#include "game/core/enums.h"

#include <vector>
#include <memory>
#include <string>
#include <game/properties/MaterialDef.h>

namespace game
{
    class BodySection;
    class Item;
    class Attack;
    class Weapon;
}

namespace properties
{
    struct BodyPartDef;
}

namespace game {
class BodyPart : public std::enable_shared_from_this<BodyPart>
{
    public:
        BodyPart();
        ~BodyPart();

        static std::shared_ptr<BodyPart> create(std::shared_ptr<BodySection> section,
                std::shared_ptr<const properties::BodyPartDef> aBodyPartDef,
                std::string nameModifier="", std::shared_ptr<BodyPart> containedBy=nullptr,
                std::shared_ptr<Item> prostheticPart=nullptr);

        void pre_delete();

        const BodyFunction& function() const;
        void addFunction();
        void regainFunction();
        void damageFunction();
        bool hasFunction(const BodyFunction& func) const;

        bool hasBodyProperty(const BodyPartProperty& property) const;

        void dependantPartLost(std::shared_ptr<BodyPart> bodyPart);
        void dependantPartDamaged(std::shared_ptr<BodyPart> bodyPart);

        bool hasStatus(const BodyPartStatus& status) const;
        void addStatus(const BodyPartStatus& status);

        std::shared_ptr<const properties::MaterialDef> material() const;

        float toHitWeight() const;

        void damage(std::shared_ptr<Attack> attack, size_t damageTypeIndex, std::vector<std::string>& hitEffects);

        std::shared_ptr<BodySection> section() const;

        std::string name() const;

        void damageFunctionThroughArmor(const float& value);

        bool isSevered() const;
        bool isCompletelyDestroyed() const;
        bool isBleeding() const;
        bool hasInternalBleeding() const;
        bool isArteryStruck() const;
        size_t bleedingCount() const;
        size_t damagedArteriesCount() const;

        void disable();

        std::shared_ptr<Item> prostheticPart() const;
        std::shared_ptr<Item> removeProstheticPart();

        void healWound();
    private:
        void init(std::shared_ptr<BodySection> section, std::shared_ptr<const properties::BodyPartDef> aBodyPartDef,
                std::string nameModifier="", std::shared_ptr<BodyPart> containedBy=nullptr,
                std::shared_ptr<Item> prostheticPart=nullptr);

        std::shared_ptr<BodySection> parent_section;
        std::shared_ptr<BodyPart> parent_contained;
        std::shared_ptr<Item> mProstheticPart;
        std::shared_ptr<Weapon> mNaturalWeapon;
        BodyFunction mBodyFunction;
        std::string mName;

        std::vector<BodyPartStatus> mStatus;
        std::vector<BodyPartProperty> mBodyProperties;

        std::shared_ptr<const properties::BodyPartDef> mBodyPartDef;

        std::vector<std::shared_ptr<BodyPart>> mContainedParts;

        std::shared_ptr<const properties::MaterialDef> mMaterial;

        std::shared_ptr<BodyPart> randomContainedPart() const;

        float mToHitWeight;
        float mDamageTaken;

        void lostFunction();

        void containedPartDisabled(std::shared_ptr<BodyPart> containedPart);
    };
}

#endif //ZERL_BODYPART_H
