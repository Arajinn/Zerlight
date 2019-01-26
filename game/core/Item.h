//
// Created by tatiana on 08.06.2018.
//

#ifndef ZERL_ITEM_H
#define ZERL_ITEM_H

#include "game/core/types.h"
#include "game/core/GameEntity.h"

namespace properties
{
    struct ItemDefinition;
}

namespace game
{
    class ItemHistory;
    class Character;
}

namespace game {
    class Item : public GameEntity{
    public:
        Item(const map::vector3& position);
        virtual ~Item();

        static std::shared_ptr<Item> create(const map::vector3& position, const std::string& id, const MaterialID_t& materialID,
                std::shared_ptr<const properties::ItemDefinition> aItemDef);

        virtual void spawn(std::shared_ptr<map::MapCell> mapCell) override;

        virtual void moveTo(map::vector3 new_position, bool stopPathing=false) override;

        virtual void toDestroy();
        virtual void toDeconstruct();
        virtual void pre_delete();

        std::string itemID() const;
        MaterialID_t materialID() const;

        ItemQuality quality() const;

        unsigned int value() const;

        virtual bool claimed() const;
        void claim(std::shared_ptr<Character> owner);
        const std::shared_ptr<Character> claimedBy() const;

        const bool& inStockpile() const;
        void setInStockpile(bool value);

        float effectAmount(ItemEffectType type) override;

        virtual void abandon();

        const std::shared_ptr<GameEntity> parent() const;
        void setParent(std::shared_ptr<GameEntity> ent);

        float combatValue() const;
        float movementPenalty() const;
        float jobPenalty() const;

        virtual bool isUpdatable() const;

        virtual std::vector<properties::TileDef> get_draw_info(const int& x, const int& y, const int& z,
                                                     const view_orientation& orientation) const;

        std::string materialName() const;
        std::string name() const;
        bool hasUniqueName() const;

        std::shared_ptr<const properties::ItemDefinition> itemDef() const;

        float damageTaken() const;
        void setDamageTaken(const float& value);

        std::shared_ptr<game::ItemHistory> history() const;

        void repair();
    protected:
        virtual void init(const std::string& id, const MaterialID_t& materialID, std::shared_ptr<const properties::ItemDefinition> aItemDef);

    private:
        std::shared_ptr<game::ItemHistory> mHistory;
        std::shared_ptr<const properties::ItemDefinition> mItemDef;
        std::shared_ptr<GameEntity> mParent;

        std::shared_ptr<Character> mCharacter;

        bool mInStockpile;

        float mDamageTaken;
    };
}

#endif //ZERL_ITEM_H
