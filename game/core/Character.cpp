//
// Created by tatiana on 26.03.2018.
//

#include "Character.h"
#include "defines.h"
#include "Pathfinder.h"
#include "Body.h"
#include "Mind.h"
#include "Squad.h"
#include "Job.h"
#include "Item.h"
#include "Construction.h"
#include "CharacterHistory.h"
#include "GameManager.h"
#include "Fortress.h"
#include "game/map/Region.h"
#include "game/map/Map.h"
#include "game/map/MapCell.h"
#include "game/map/NavGraphNode.h"
#include "game/properties/GameDefinition.h"
#include "game/properties/RaceDefinition.h"
#include "game/properties/ItemDefinition.h"
#include "game/properties/GameSettings.h"
#include "game/utils/Randomizer.h"
#include "game/behavior/Node.h"
#include "game/behavior/PlayerCharacterBehavior.h"

#include <algorithm>

namespace game
{
    Character::Character(const map::vector3& position)
        :GameEntity(position)
    {

    }

    Character::~Character()
    {

    }

    std::shared_ptr<Character> Character::create(const map::vector3& position,
            std::shared_ptr<const properties::RaceDefinition> aRaceDef)
    {
        std::shared_ptr<Character> ptr=std::shared_ptr<Character>(new Character(position));
        ptr->init(aRaceDef);
        return ptr;
    }

    void Character::init(std::shared_ptr<const properties::RaceDefinition> aRaceDef)
    {
        mSquad=nullptr;
        mJob=nullptr;
        mMoveTime=0.0f;
        mCastLightTimer=0.0f;
        mStartingFallPosition=map::vector3_one;
        mClaimedBy=nullptr;
        mRootBehavior=nullptr;
        mBehaviorType=behavior::BehaviorType::None;
        mCurrentNeedGoal=nullptr;

        mHistory=std::shared_ptr<CharacterHistory>(new CharacterHistory(aRaceDef));
        mHistory->setName("Test settler");

        mPathfinder=std::shared_ptr<Pathfinder>(new Pathfinder(std::dynamic_pointer_cast<Character>(shared_from_this())));
        body=Body::create(std::dynamic_pointer_cast<Character>(shared_from_this()));
        mind=Mind::create(std::dynamic_pointer_cast<Character>(shared_from_this()));

        mAttributes.resize(aRaceDef->Attributes.size());
        for (int i=0,isize=mAttributes.size();i<isize;i++)
        {
            float min=float(aRaceDef->Attributes.at(i).Min);
            float max=float(aRaceDef->Attributes.at(i).Max);
            float mean=float(aRaceDef->Attributes.at(i).Mean);
            float value=RANDOMIZER->norm(min,max,mean,3.0f);
            mAttributes[i]=CharacterAttribute(int(value));
        }
    }

    void Character::spawn(std::shared_ptr<map::MapCell> mapCell)
    {
        mapCell->addCharacter(std::dynamic_pointer_cast<Character>(shared_from_this()));
        GameEntity::spawn(mapCell);
    }

    float Character::formationMoveSpeedBonus() const
    {
        if (mSquad== nullptr)
            return 1.0f;
        else
            return mSquad->formationMoveSpeedBonus();
    }

    float Character::attributeLevel(CharacterAttributeType attribute) const
    {
        return float(mAttributes.at(int(attribute)).AttributeLevel)/100.0f;
    }

    std::shared_ptr<SquadPosition> Character::squadPosition() const
    {
        if (mSquad== nullptr)
            return nullptr;

        return mSquad->position(this);
    }

    std::shared_ptr<Pathfinder> Character::pathfinder() const
    {
        return mPathfinder;
    }

    std::shared_ptr<const properties::RaceDefinition> Character::raceDefinition() const
    {
        return mHistory->raceDef;
    }

    std::shared_ptr<CharacterHistory> Character::history() const
    {
        return mHistory;
    }

    void Character::cancelJob(std::string reason)
    {

    }

    bool Character::pathTo(map::vector3 position, bool adjacent)
    {
        if (!mPathfinder->findNavPath(this->position(),position,adjacent))
            return false;

        auto map=WORLD_MAP;
        auto cell1=map->cell(this->position());
        auto cell2=map->cell(mPathfinder->endPosition());

        if ((cell1->willSuffocate()) || (cell1->hasLava()) || (!cell2->willSuffocate()) || (!cell2->hasLava()))
            return true;

        mPathfinder->clearPath();

        return false;
    }

    void Character::moveTo(map::vector3 new_position)
    {
        auto map=WORLD_MAP;

        if ((mStartingFallPosition==map::vector3_one) && (!map->isWalkable(new_position)) && (this->cell()->hasFloor()))
        {
            mStartingFallPosition=this->position();
            mPathfinder->clearPath();
            mMoveTime=0.0f;
        }

        GameEntity::moveTo(new_position);
    }

    void Character::pre_update()
    {
        //if (mPathfinder->pathing())
        //    mPathfinder->stepPath();

        GameEntity::pre_update();
    }

    void Character::update(float dt)
    {
        if (body->isDead())
            return;

        if (this->updateBody(dt))
        {

        }

        if (mRootBehavior!= nullptr)
        {
            mRootBehavior->execute(std::dynamic_pointer_cast<Character>(shared_from_this()),dt);
        }

        if (mPathfinder->pathing())
        {
            float speed=body->moveSpeed()*this->formationMoveSpeedBonus();

            if (mJob!= nullptr)
            {
                if (mJob->requiredCharacter()!= nullptr)
                {
                    auto shilf=this->position()-mJob->requiredCharacter()->position();
                    if (shilf.lengthSquared()<=2.0f)
                    {
                        if (mJob->requiredCharacter()->body== nullptr)
                            this->cancelJob();
                        else if (mJob->requiredCharacter()->body->isDead())
                            this->cancelJob();
                        else
                            speed=std::min(speed,mJob->requiredCharacter()->body->moveSpeed());
                    }
                }
            }

            mMoveTime+=speed*dt;

            while (mPathfinder->pathing() && (mMoveTime>1.0f))
            {
                mCastLightTimer=0.0f;
                mMoveTime-=1.0f;

                auto nextPos=mPathfinder->nextPosition();
                if (nextPos!=this->position()) {
                    auto mapCell = WORLD_MAP->cell(nextPos);

                    if (WORLD_MAP->isWalkable(nextPos) || (mJob != nullptr) && (mapCell->proposedJob() == mJob))
                    {
                        if (mPathfinder->pathing())
                        {
                            if (mJob != nullptr)
                            {
                                if (mJob->requiredCharacter() != nullptr)
                                {
                                    if ((this->position() - mJob->requiredCharacter()->position()).lengthSquared() <= 2.0f)
                                    {
                                        mJob->requiredCharacter()->moveTo(this->position());
                                    }
                                }
                            }

                            this->moveTo(nextPos);
                            if (!mPathfinder->stepPath())
                            {
                                mMoveTime=0.0f;
                                mPathfinder->clearPath();
                            }
                        }
                    }
                }
            }
        }

        GameEntity::update(dt);
    }

    bool Character::claimed() const
    {
        return (mClaimedBy!= nullptr);
    }

    void Character::setBehavior(behavior::BehaviorType type)
    {
        mBehaviorType=type;
        mRootBehavior= nullptr;

        if (type==behavior::BehaviorType::PlayerCharacter)
        {
            mRootBehavior=std::shared_ptr<behavior::PlayerCharacterBehavior>(new behavior::PlayerCharacterBehavior());
        }
    }

    bool Character::isThirsty() const
    {
        return body->isThirsty();
    }

    bool Character::isDyingOfThirst() const
    {
        return body->isDyingOfThirst();
    }

    const bool& Character::isLookingForDrink() const
    {
        return body->isLookingForDrink();
    }

    void Character::setLookingForDrink()
    {
        body->setLookingForDrink();
    }

    std::shared_ptr<GameEntity> Character::currentNeedGoal() const
    {
        return mCurrentNeedGoal;
    }

    std::shared_ptr<Job> Character::job() const
    {
        return mJob;
    }

    map::vector3 Character::desiredPosition() const
    {
        return mPathfinder->endPosition();
    }

    bool Character::atEndPosition() const
    {
        return mPathfinder->atEndPosition(this->position());
    }

    bool Character::canReach(const map::vector3& pos, bool adjacent)
    {
        return mPathfinder->canReach(this->position(), pos, adjacent);
    }

    void Character::abandonNeedGoal()
    {
        if (mCurrentNeedGoal!= nullptr)
        {
            int t=0;
        }

        mCurrentNeedGoal=nullptr;
    }

    bool Character::tryToUseItem(std::shared_ptr<Item> item)
    {
        if (item== nullptr)
            return false;

        if (item->claimed())
            return false;

        if (!this->canReach(item->position(),false))
            return false;

        this->abandonNeedGoal();
        mCurrentNeedGoal=item;
        item->claim(std::dynamic_pointer_cast<Character>(shared_from_this()));

        return true;
    }

    bool Character::findOwnedConstruction(ItemEffectType type)
    {
        return false;
    }

    bool Character::findOwnedPossession(ItemEffectType type)
    {
        auto gameDef=GAME_DEFINITIONS;

        for (auto posession : mPossessions)
        {
            if ((gameDef->itemDefinition(posession->itemID())->hasEffect(type)))
                return this->tryToUseItem(posession);
        }

        return false;
    }

    bool Character::findEntity(ItemEffectType type)
    {
        if (this->findOwnedConstruction(type))
            return true;

        if (this->findOwnedPossession(type))
            return true;

        auto entities=GMINSTANCE->region()->fortress()->getEntities(type);
        if (entities.size()==0)
            return false;

        if (entities.size()>1)
        {
            if ((type==ItemEffectType::Drink) || (type==ItemEffectType::Food))
            {
                float nutritionWeight=GAME_SETTINGS->NutritionWeight;
                map::vector3 pos=this->position();
                std::sort(std::begin(entities),std::end(entities),[&nutritionWeight,&type,&pos](const std::shared_ptr<GameEntity>& left,const std::shared_ptr<GameEntity>& right)
                {
                    float num1;
                    float effect1=left->effectAmount(type);
                    if (effect1>0.0)
                        num1=nutritionWeight/effect1;
                    else
                        num1=1e9;

                    float mapWeight1=map::Map::distanceSquaredWithBias(pos,left->position());

                    float num2;
                    float effect2=right->effectAmount(type);
                    if (effect2>0.0)
                        num2=nutritionWeight/effect2;
                    else
                        num2=1e9;

                    float mapWeight2=map::Map::distanceSquaredWithBias(pos,right->position());

                    float weight1=mapWeight1+mapWeight1*num1*num1+1.0f/effect1;
                    float weight2=mapWeight2+mapWeight2*num2*num2+1.0f/effect2;

                    return weight1<weight2;
                });
            }
            else
            {
                map::vector3 pos=this->position();
                std::sort(std::begin(entities),std::end(entities),[&type,&pos](const std::shared_ptr<GameEntity>& left,const std::shared_ptr<GameEntity>& right)
                {
                    float effect1=left->effectAmount(type);
                    float effect2=right->effectAmount(type);

                    if (effect1==effect2)
                    {
                        float mapWeight1=map::Map::distanceSquaredWithBias(pos,left->position());
                        float mapWeight2=map::Map::distanceSquaredWithBias(pos,right->position());

                        return mapWeight1<mapWeight2;
                    }
                    else
                        return effect1<effect2;
                });
            }
        }

        auto this_cell=WORLD_MAP->cell(this->position());
        if (this_cell->navGraphNode()==nullptr)
            return false;

        for (auto ent : entities)
        {
            auto castToItem=std::dynamic_pointer_cast<Item>(ent);
            if (castToItem)
            {
                if (GAME_DEFINITIONS->itemDefinition(castToItem->itemID())->hasEffect(type))
                {
                    auto cell=WORLD_MAP->cell(ent->position());
                    if ((cell->navGraphNode()!=nullptr) && (cell->navGraphNode()->navGraphID()==this_cell->navGraphNode()->navGraphID()) && (this->tryToUseItem(castToItem)))
                        return true;
                }
            }
            else
            {
                auto castToConstruction=std::dynamic_pointer_cast<Construction>(ent);
                if (castToConstruction)
                {

                }
            }
        }

        return false;
    }

    void Character::pickingUpItem(std::shared_ptr<Item> item)
    {
        if (item->parent()!= nullptr)
        {

        }

        if (mCurrentNeedGoal->ID()==item->ID())
            mCurrentNeedGoal=nullptr;

        item->claim(std::dynamic_pointer_cast<Character>(shared_from_this()));
        item->cell()->removeObject(item);
        item->setParent(std::dynamic_pointer_cast<Character>(shared_from_this()));
        item->setInStockpile(false);
    }

    bool Character::pickupItem(std::shared_ptr<Item> item)
    {
        if (!body->pickupItem(item))
        {
            if (item->claimedBy()!= nullptr)
            {
                if (item->claimedBy()->ID() == this->ID())
                    item->abandon();
            }

            return false;
        }

        this->pickingUpItem(item);
        return true;
    }

    float Character::abilityLevel(BodyFunction bodyFunction)
    {
        return body->abilityLevel(bodyFunction);
    }

    void Character::drinkItem(std::shared_ptr<Item> item)
    {
        body->drinkItem(item);
        mind->adjustHappiness(2.0f*properties::ItemDefinition::effectModifierForFoodAndDrink(item->quality()));
        item->toDestroy();
    }

    void Character::setNeedGoal(std::shared_ptr<GameEntity> ent)
    {
        this->abandonNeedGoal();
        mCurrentNeedGoal=ent;
    }

    bool Character::updateBody(float dt)
    {
        if (body->update(dt))
        {
            return true;
        }

        return false;
    }

    std::vector<std::shared_ptr<Item>> Character::heldItems() const
    {
        return body->heldItems();
    }

    const bool& Character::isLookingForFood() const
    {
        return body->isLookingForFood();
    }

    void Character::setLookingForFood()
    {
        body->setLookingForFood();
    }

    bool Character::hasItem(std::shared_ptr<Item> item) const
    {
        return body->hasItem(item);
    }

    bool Character::canCarry(std::shared_ptr<Item> item) const
    {
        return body->canCarry(item);
    }

    bool Character::canCarry(EquipmentType equipType) const
    {
        return body->canCarry(equipType);
    }

    bool Character::sheathItem()
    {
        return body->sheathItem();
    }

    void Character::actuallyDrop(std::shared_ptr<Item> item)
    {
        item->abandon();
        item->setParent(nullptr);
        item->moveTo(this->position());
    }

    bool Character::dropItem(EquipmentType equipSlot)
    {
        auto list=body->getDropedItem(equipSlot);
        for (auto item : list)
        {
            this->actuallyDrop(item);
        }

        return (list.size()>0);
    }

    void Character::eatFoot(std::shared_ptr<Item> item)
    {
        body->eatFoot(item);

        auto cell=WORLD_MAP->cell(this->position());
        if (cell->hasEmbeddedWall())
        {
            if (cell->embeddedWall()->effectAmount(ItemEffectType::Sit)>0.0f)
                mind->adjustHappiness(2.0f*properties::ItemDefinition::effectModifier(item->quality()));
            else
                mind->adjustHappiness(-5.0f);
        }
        else
            mind->adjustHappiness(-5.0f);


        item->toDestroy();
    }

    bool Character::isHungry() const
    {
        return body->isHungry();
    }

    bool Character::isStarving() const
    {
        return body->isStarving();
    }

    bool Character::isUpdatable() const
    {
        return true;
    }
}