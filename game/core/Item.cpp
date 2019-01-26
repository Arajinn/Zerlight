//
// Created by tatiana on 08.06.2018.
//

#include "Item.h"
#include "defines.h"
#include "ItemHistory.h"
#include "GameManager.h"
#include "Fortress.h"
#include "game/map/Region.h"
#include "game/properties/GameDefinition.h"
#include "game/properties/ItemDefinition.h"
#include "game/properties/MaterialDef.h"
#include "game/map/MapCell.h"

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

    std::shared_ptr<Item> Item::create(const map::vector3& position, const std::string& id, const MaterialID_t& materialID,
            std::shared_ptr<const properties::ItemDefinition> aItemDef)
    {
        auto ptr=std::make_shared<Item>(position);
        ptr->init(id, materialID,aItemDef);
        return ptr;
    }

    void Item::init(const std::string& id, const MaterialID_t& materialID, std::shared_ptr<const properties::ItemDefinition> aItemDef)
    {
        mItemDef=aItemDef;
        mCharacter=nullptr;
        mInStockpile=false;
        mParent=nullptr;
        mDamageTaken=0.0f;

        mHistory=std::make_shared<ItemHistory>(std::dynamic_pointer_cast<Item>(shared_from_this()), id, materialID);
    }

    void Item::pre_delete()
    {
        //mItemDef= nullptr;
        mCharacter=nullptr;
        mParent=nullptr;

        mHistory->pre_delete();
        //mHistory=nullptr;
    }

    void Item::spawn(std::shared_ptr<map::MapCell> mapCell)
    {
        //GameEntity::spawn(mapCell);
        mapCell->addObject(shared_from_this());
    }

    std::string Item::itemID() const
    {
        return mHistory->itemID();
    }

    MaterialID_t Item::materialID() const
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

        GMINSTANCE->region()->fortress()->removeItem(std::dynamic_pointer_cast<Item>(shared_from_this()));
        GMINSTANCE->addToDeleteList(std::dynamic_pointer_cast<Item>(shared_from_this()));

        for (const auto& component : mHistory->components())
        {
            component->setParent(nullptr);
            component->toDestroy();
        }

        mHistory->clearComponents();
    }

    void Item::toDeconstruct()
    {
        if (!this->isValid())
            return;

        mParent=nullptr;

        GMINSTANCE->region()->fortress()->removeItem(std::dynamic_pointer_cast<Item>(shared_from_this()));
        GMINSTANCE->addToDeleteList(std::dynamic_pointer_cast<Item>(shared_from_this()));

        for (const auto& component : mHistory->components())
        {
            component->setParent(nullptr);
            component->moveTo(position(),false);
            GMINSTANCE->region()->fortress()->addItem(component);
        }

        mHistory->clearComponents();
    }

    float Item::combatValue() const
    {
        float strength=GAME_DEFINITIONS->materialDefinition(mHistory->materialID())->Strength;
        float modifier=mItemDef->CombatRatingModifier;
        return modifier*strength;
    }

    float Item::movementPenalty() const
    {
        return mItemDef->EquippedMovePenalty;
    }

    float Item::jobPenalty() const
    {
        return mItemDef->EquippedJobPenalty;
    }

    void Item::moveTo(map::vector3 new_position, bool stop_pathing)
    {
        GameEntity::moveTo(new_position);
    }

    bool Item::isUpdatable() const
    {
        return false;
    }

    std::vector<properties::TileDef> Item::get_draw_info(const int& x, const int& y, const int& z,
                                                 const view_orientation& orientation) const
    {
        std::vector<properties::TileDef> result;
        float order=0.0f;
        for (const auto& item : mItemDef->Sprites)
        {
            properties::TileDef info;
            info.DrawOrder=order;
            if (item.spriteIDByMaterialID.empty())
            {
                info.SpriteID=item.spriteID;
            }
            else
            {
                const auto materialID = mHistory->materialID();
                const std::string spriteID=item.getSpriteIDByMaterialID(materialID);
                if (!spriteID.empty())
                {
                    info.SpriteID=spriteID;
                }
                else
                {
                    info.SpriteID=item.spriteID;
                }
            }

            if (item.useMaterial)
            {
                const auto materialID = mHistory->materialID();
                std::shared_ptr<const properties::MaterialDef> materialDef=GAME_DEFINITIONS->materialDefinition(materialID);
                info.Color=materialDef->Color;
            }
            else
                info.Color=gui::ZColor(-1,-1,-1);

            result.push_back(info);

            order+=0.1f;
        }

        return result;
    }

    std::string Item::materialName() const
    {
        return mHistory->materialName();
    }

    std::string Item::name() const
    {
        return mHistory->name();
    }

    bool Item::hasUniqueName() const
    {
        return mHistory->hasUniqueName();
    }

    std::shared_ptr<const properties::ItemDefinition> Item::itemDef() const
    {
        return mItemDef;
    }

    float Item::damageTaken() const
    {
        return mDamageTaken;
    }

    void Item::setDamageTaken(const float& value)
    {
        mDamageTaken=value;
    }

    std::shared_ptr<game::ItemHistory> Item::history() const
    {
        return mHistory;
    }

    void Item::repair()
    {
        mDamageTaken=0.0f;
    }
}