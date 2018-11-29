//
// Created by tatiana.polozova on 08.06.2018.
//

#include "Item.h"
#include "ItemHistory.h"
#include "GameManager.h"
#include "Fortress.h"
#include "game/map/Region.h"
#include "game/properties/GameDefinition.h"
#include "game/properties/ItemDefinition.h"
#include "game/properties/MaterialDef.h"

#include <iostream>

namespace game
{
    Item::Item(const map::vector3& position)
        :GameEntity(position)
    {

    }

    Item::~Item()
    {
        mHistory=nullptr;
        mItemDef=nullptr;
        mParent=nullptr;
        mCharacter= nullptr;

        std::cout << "~Item()" << std::endl;
    }

    std::shared_ptr<Item> Item::create(const map::vector3& position, std::string id, std::string materialID, std::shared_ptr<properties::ItemDefinition> aItemDef)
    {
        std::shared_ptr<Item> ptr=std::shared_ptr<Item>(new Item(position));
        ptr->init(id, materialID,aItemDef);
        return ptr;
    }

    void Item::init(std::string id, std::string materialID, std::shared_ptr<properties::ItemDefinition> aItemDef)
    {
        mItemDef=aItemDef;
        mCharacter=nullptr;
        mInStockpile=false;
        mParent=nullptr;

        mHistory=std::shared_ptr<ItemHistory>(new ItemHistory(id, materialID));
    }

    void Item::spawn(std::shared_ptr<map::MapCell> mapCell)
    {
        GameEntity::spawn(mapCell);
    }

    std::string Item::itemID() const
    {
        return mHistory->itemID();
    }

    std::string Item::materialID() const
    {
        return mHistory->materialID();
    }

    ItemQuality Item::quality() const
    {
        return mHistory->quality();
    }

    unsigned int Item::value() const
    {
        return 1;
    }

    bool Item::claimed() const
    {
        return (mCharacter!=nullptr);
    }

    void Item::claim(std::shared_ptr<Character> owner)
    {
        mCharacter=owner;
    }

    const std::shared_ptr<Character> Item::claimedBy() const
    {
        return mCharacter;
    }

    const bool& Item::inStockpile() const
    {
        return mInStockpile;
    }

    void Item::setInStockpile(bool value)
    {
        mInStockpile=value;
    }

    float Item::effectAmount(ItemEffectType type)
    {
        if ((type==ItemEffectType::Drink) || (type==ItemEffectType::Food))
            return mItemDef->effectAmount(type)*properties::ItemDefinition::effectModifierForFoodAndDrink(mHistory->quality());
        else
            return mItemDef->effectAmount(type)*properties::ItemDefinition::effectModifier(mHistory->quality());
    }

    void Item::abandon()
    {
        mCharacter= nullptr;
    }

    const std::shared_ptr<GameEntity> Item::parent() const
    {
        return mParent;
    }

    void Item::setParent(std::shared_ptr<GameEntity> ent)
    {
        mParent=ent;
    }

    void Item::toDestroy()
    {
        if (!this->isValid())
            return;

        mParent=nullptr;

        GAME->region()->fortress()->removeItem(std::dynamic_pointer_cast<Item>(shared_from_this()));
        GAME->addToDeleteList(std::dynamic_pointer_cast<Item>(shared_from_this()));
    }

    float Item::combatValue() const
    {
        return mItemDef->CombatRatingModifier*GAME->gameDefinition()->materialDefinition(mHistory->materialID())->Strength;
    }

    float Item::movementPenalty() const
    {
        return mItemDef->EquippedMovePenalty;
    }

    float Item::jobPenalty() const
    {
        return mItemDef->EquippedJobPenalty;
    }

    void Item::moveTo(map::vector3 new_position)
    {
        GameEntity::moveTo(new_position);
    }
}