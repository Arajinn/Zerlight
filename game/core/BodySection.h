//
// Created by tatiana on 13.06.2018.
//

#ifndef ZERL_BODYSECTION_H
#define ZERL_BODYSECTION_H

#include "game/core/enums.h"
#include "game/core/types.h"
#include "game/properties/TileDef.h"

#include <vector>
#include <memory>

namespace game
{
    class Body;
    class BodyPart;
    class Item;
    class Attack;
    class Weapon;
}

namespace properties
{
    class BodySectionDef;
}

namespace game {
    class BodySection : public std::enable_shared_from_this<BodySection> {
    public:
        BodySection();
        ~BodySection();

        static std::shared_ptr<BodySection> create(unsigned int &key, std::shared_ptr<Body> parent,
                std::shared_ptr<properties::BodySectionDef> sectionDef, BodySymmetricalType symmetricalType=BodySymmetricalType::None);

        void addFunction(std::shared_ptr<BodyPart> bodyPart);
        void regainFunction(std::shared_ptr<BodyPart> bodyPart);
        void lostFunction(std::shared_ptr<BodyPart> bodyPart);
        void damageFunction(std::shared_ptr<BodyPart> bodyPart);

        bool canCarry() const;
        void carryItem(std::shared_ptr<Item> item, bool alreadyCheck=false);

        std::shared_ptr<Item> heldItem() const;

        void dropItem();

        const EquipmentType& equipType() const;
        std::shared_ptr<Item> equippedItem() const;

        void getTiles(std::vector<properties::TileDef>& tiles) const;

        unsigned int ID() const;

        bool hasStatus(const BodySectionStatus& status) const;

        const float& toHitWeight() const;

        bool hasFunction(const BodyFunction& function) const;
        bool directlyConnectsToFunction(const BodyFunction& function) const;

        std::shared_ptr<BodyPart> bodyPart() const;

        void damageEquipment(std::shared_ptr<Attack> attack, size_t damageTypeIndex, std::vector<std::string>& hitEffects);

        std::shared_ptr<Body> body() const;

        void addWeapon(std::shared_ptr<Weapon> weapon);
        void removeWeapon(std::shared_ptr<Weapon> weapon);

        void unequip(std::shared_ptr<Item> item);

        void damage(std::shared_ptr<Attack> attack, std::vector<std::string>& hitEffects);

        std::shared_ptr<Item> removeArtificialPart();

        std::string bodyPartID() const;

        float bloodLossRate() const;

        std::vector<BodySectionStatus> status() const;

        std::string name() const;

        bool isConnection() const;
        std::vector<std::shared_ptr<BodySection>> connectsTo() const;

        void repairEquipment();

        MaterialID_t materialID() const;
    private:
        void init(unsigned int &key, std::shared_ptr<Body> parent, std::shared_ptr<properties::BodySectionDef> sectionDef,
                BodySymmetricalType symmetricalType);

        unsigned int mID;

        std::string mName;

        std::shared_ptr<Body> parent_body;
        std::string mBodyPartID;
        std::shared_ptr<BodyPart> mBodyPart;
        std::shared_ptr<BodyPart> mArtificialPart;
        bool mRemoveArtificialLimb;
        std::vector<std::shared_ptr<BodySection>> mConnectsTo;
        bool mConnection;
        bool mHasGrip;
        std::shared_ptr<Item> mHeldItem;
        EquipmentType mEquipType;
        std::shared_ptr<Item> mEquippedItem;
        std::string mSectionSpriteID;
        std::vector<std::string> mAdditionalDecorations;
        int mDrawOrder;
        BodySymmetricalType mSymmetricalType;
        bool mLimb;
        float mBloodLossRate;
        MaterialID_t mMaterialID;

        std::vector<BodySectionStatus> mStatus;
        void addStatus(const BodySectionStatus& status);
        void removeStatus(const BodySectionStatus& status);

        float mToHitWeight;

        float equipmentQualityModifier() const;

        void checkStatus();

        void setMissing();
    };
}

#endif //ZERL_BODYSECTION_H
