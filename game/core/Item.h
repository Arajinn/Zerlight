//
// Created by tatiana on 08.06.2018.
//

#ifndef ZERL_ITEM_H
#define ZERL_ITEM_H

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
        ~Item();

        static std::shared_ptr<Item> create(const map::vector3& position, std::string id, std::string materialID,
                std::shared_ptr<const properties::ItemDefinition> aItemDef);

        virtual void spawn(std::shared_ptr<map::MapCell> mapCell) override;

        virtual void moveTo(map::vector3 new_position) override;

        virtual void toDestroy();

        std::string itemID() const;
        std::string materialID() const;

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
    protected:
        virtual void init(std::string id, std::string materialID, std::shared_ptr<const properties::ItemDefinition> aItemDef);

    private:
        std::shared_ptr<game::ItemHistory> mHistory;
        std::shared_ptr<const properties::ItemDefinition> mItemDef;
        std::shared_ptr<GameEntity> mParent;

        std::shared_ptr<Character> mCharacter;

        bool mInStockpile;
    };
}

#endif //ZERL_ITEM_H
