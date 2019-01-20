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
#include "ItemHistory.h"
#include "Construction.h"
#include "CharacterHistory.h"
#include "GameManager.h"
#include "Fortress.h"
#include "SquadPosition.h"
#include "Formation.h"
#include "Stairs.h"
#include "Weapon.h"
#include "Attack.h"
#include "TrainOrder.h"
#include "Faction.h"
#include "AIDirector.h"
#include "Military.h"
#include "TrainingStation.h"
#include "Enums2Str.h"
#include "StringConstants.h"
#include "game/map/Region.h"
#include "game/map/Map.h"
#include "game/map/MapCell.h"
#include "game/map/NavGraphNode.h"
#include "game/properties/GameDefinition.h"
#include "game/properties/RaceDefinition.h"
#include "game/properties/ItemDefinition.h"
#include "game/properties/GameSettings.h"
#include "game/properties/RaceClassDef.h"
#include "game/properties/UniformSettings.h"
#include "game/properties/ItemSettings.h"
#include "game/properties/AttackDef.h"
#include "game/properties/MaterialDef.h"
#include "game/properties/WeaponDef.h"
#include "game/properties/CharacterSettings.h"
#include "game/properties/DefendDef.h"
#include "game/properties/AmmoDef.h"
#include "game/properties/ItemSettings.h"
#include "game/utils/Randomizer.h"
#include "game/behavior/Node.h"
#include "game/behavior/PlayerCharacterBehavior.h"
#include "game/behavior/WildAnimalBehavior.h"

#include <algorithm>
#include <cfloat>

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
            std::shared_ptr<const properties::RaceDefinition> aRaceDef, std::shared_ptr<const properties::FactionDef> aFactionDef,
                                                 const size_t& factionID)
    {
        std::shared_ptr<Character> ptr=std::make_shared<Character>(position);
        ptr->init(aRaceDef,aFactionDef,factionID);
        return ptr;
    }

    void Character::init(std::shared_ptr<const properties::RaceDefinition> aRaceDef,
            std::shared_ptr<const properties::FactionDef> aFactionDef, const size_t& factionID)
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
        mCurrentTarget=nullptr;
        mLastAttacker=nullptr;
        mRecentlyRanTimer=0.0f;
        mTrackEnemy=false;
        mHasBeenSpotted=false;

        mHistory=std::make_shared<CharacterHistory>(aRaceDef,aFactionDef, factionID);

        if (aRaceDef->ID=="Gnome")
            mHistory->setName("Test Gnome");
        else if (aRaceDef->ID=="Bear")
            mHistory->setName("Wild Bear");

        mPathfinder=std::make_shared<Pathfinder>(std::dynamic_pointer_cast<Character>(shared_from_this()));
        mBody=Body::create(std::dynamic_pointer_cast<Character>(shared_from_this()));
        mMind=Mind::create(std::dynamic_pointer_cast<Character>(shared_from_this()));

        mAttributes.resize(aRaceDef->Attributes.size());
        for (size_t i=0,isize=mAttributes.size();i<isize;i++)
        {
            const auto min=float(aRaceDef->Attributes.at(i).Min);
            const auto max=float(aRaceDef->Attributes.at(i).Max);
            const auto mean=float(aRaceDef->Attributes.at(i).Mean);
            const auto value=RANDOMIZER->norm(min,max,mean,3.0f);
            mAttributes[i]=CharacterAttribute(int(value));
        }
    }

    void Character::setID(const int& value)
    {
        GameEntity::setID(value);

        mHistory->setCharaterTD(value);
    }

    void Character::spawn(std::shared_ptr<map::MapCell> mapCell)
    {
        mapCell->addCharacter(std::dynamic_pointer_cast<Character>(shared_from_this()));
        //GameEntity::spawn(mapCell);
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
        return mHistory->attributeLevel(attribute);
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
        return mHistory->raceDef();
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

    void Character::moveTo(map::vector3 new_position, bool stop_pathing)
    {
        if ((mStartingFallPosition==map::vector3_one) && (!WORLD_MAP->isWalkable(new_position)) && (this->cell()->hasFloor()))
        {
            mStartingFallPosition=position();
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
        if (mBody->isDead())
            return;

        if (updateBody(dt))
        {

        }

        checkSurroundings(dt);

        if (mRootBehavior!= nullptr)
        {
            mRootBehavior->execute(std::dynamic_pointer_cast<Character>(shared_from_this()),dt);
        }

        if (mPathfinder->pathing())
        {
            float speed=mBody->moveSpeed()*this->formationMoveSpeedBonus();

            if (mJob!= nullptr)
            {
                if (mJob->requiredCharacter()!= nullptr)
                {
                    if (map::vector3::distance(position(),mJob->requiredCharacter()->position())<=2.0f)
                    {
                        if (mJob->requiredCharacter()->body()== nullptr)
                            cancelJob();
                        else if (mJob->requiredCharacter()->body()->isDead())
                            cancelJob();
                        else
                            speed=std::min(speed,mJob->requiredCharacter()->body()->moveSpeed());
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
                                    if ((this->position() - mJob->requiredCharacter()->position()).length() <= 2.0f)
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
            mRootBehavior=std::make_shared<behavior::PlayerCharacterBehavior>();
        }
        else if (type==behavior::BehaviorType::WildAnimal)
        {
            mRootBehavior=std::make_shared<behavior::WildAnimalBehavior>();
        }
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

    bool Character::canReach(const map::vector3& pos, bool adjacent) const
    {
        return mPathfinder->canReach(this->position(), pos, adjacent);
    }

    void Character::abandonNeedGoal()
    {
        if (mCurrentNeedGoal!= nullptr)
        {

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

        for (const auto& posession : mPossessions)
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
        if (entities.empty())
            return false;

        if (entities.size()>1)
        {
            if ((type==ItemEffectType::Drink) || (type==ItemEffectType::Food))
            {
                float nutritionWeight=GAME_SETTINGS->NutritionWeight;
                map::vector3 pos=position();
                std::sort(entities.begin(),entities.end(),
                        [&nutritionWeight,&type,&pos](const std::shared_ptr<GameEntity>& left,const std::shared_ptr<GameEntity>& right)
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
                map::vector3 pos=position();
                std::sort(entities.begin(),entities.end(),
                        [&type,&pos](const std::shared_ptr<GameEntity>& left,const std::shared_ptr<GameEntity>& right)
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

        auto this_cell=WORLD_MAP->cell(position());
        if (this_cell->navGraphNode()==nullptr)
            return false;

        for (const auto& ent : entities)
        {
            auto castToItem=std::dynamic_pointer_cast<Item>(ent);
            if (castToItem)
            {
                if (GAME_DEFINITIONS->itemDefinition(castToItem->itemID())->hasEffect(type))
                {
                    auto cell=WORLD_MAP->cell(ent->position());
                    if ((cell->navGraphNode()!=nullptr) && (cell->navGraphNode()->navGraphID()==this_cell->navGraphNode()->navGraphID())
                        && (tryToUseItem(castToItem)))
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
        if (!mBody->pickupItem(item))
        {
            if (item->claimedBy()!= nullptr)
            {
                if (item->claimedBy()->ID() == ID())
                    item->abandon();
            }

            return false;
        }

        this->pickingUpItem(item);
        return true;
    }

    float Character::abilityLevel(BodyFunction bodyFunction)
    {
        return mBody->abilityLevel(bodyFunction);
    }

    void Character::drinkItem(std::shared_ptr<Item> item)
    {
        mBody->drinkItem(item);
        mMind->adjustHappiness(2.0f*properties::ItemDefinition::effectModifierForFoodAndDrink(item->quality()));
        item->toDestroy();
    }

    void Character::setNeedGoal(std::shared_ptr<GameEntity> ent)
    {
        abandonNeedGoal();
        mCurrentNeedGoal=ent;
    }

    bool Character::updateBody(float dt)
    {
        if (mBody->update(dt))
        {
            return true;
        }

        return false;
    }

    std::vector<std::shared_ptr<Item>> Character::heldItems() const
    {
        return mBody->heldItems();
    }

    size_t Character::heldItemsCount() const
    {
        return mBody->heldItemsCount();
    }

    std::vector<std::shared_ptr<Item>> Character::equippedItems() const
    {
        return mBody->equippedItems();
    }

    size_t Character::equippedItemsCount() const
    {
        return mBody->equippedItemsCount();
    }

    const bool& Character::isLookingForFood() const
    {
        return mBody->isLookingForFood();
    }

    void Character::setLookingForFood()
    {
        mBody->setLookingForFood();
    }

    bool Character::hasItem(std::shared_ptr<Item> item) const
    {
        return mBody->hasItem(item);
    }

    bool Character::canCarry(std::shared_ptr<Item> item) const
    {
        return mBody->canCarry(item);
    }

    bool Character::canCarry(EquipmentType equipType) const
    {
        return mBody->canCarry(equipType);
    }

    bool Character::sheathItem()
    {
        return mBody->sheathItem();
    }

    void Character::actuallyDrop(std::shared_ptr<Item> item)
    {
        item->abandon();
        item->setParent(nullptr);
        item->moveTo(position(),false);
    }

    bool Character::dropItem(EquipmentType equipSlot)
    {
        auto list=mBody->getDropedItem(equipSlot);
        for (const auto& item : list)
        {
            actuallyDrop(item);
        }

        return (!list.empty());
    }

    bool Character::dropItem(std::shared_ptr<Item> item)
    {
        if (!mBody->dropItem(item))
            return false;

        actuallyDrop(item);
        return true;
    }

    void Character::eatFoot(std::shared_ptr<Item> item)
    {
        mBody->eatFoot(item);

        auto cell=WORLD_MAP->cell(position());
        if (cell->hasEmbeddedWall())
        {
            if (cell->embeddedWall()->effectAmount(ItemEffectType::Sit)>0.0f)
                mMind->adjustHappiness(2.0f*properties::ItemDefinition::effectModifier(item->quality()));
            else
                mMind->adjustHappiness(-5.0f);
        }
        else
            mMind->adjustHappiness(-5.0f);


        item->toDestroy();
    }

    bool Character::isHungry() const
    {
        return mBody->isHungry();
    }

    bool Character::isStarving() const
    {
        return mBody->isStarving();
    }

    bool Character::isUpdatable() const
    {
        return true;
    }

    std::vector<properties::TileDef> Character::get_draw_info(const int& x, const int& y, const int& z,
            const view_orientation& orientation) const
    {
        std::vector<properties::TileDef> result;

        result=mBody->getTiles();

        std::sort(result.begin(), result.end(),
                  [](const properties::TileDef & left, const properties::TileDef & right) -> bool
                  {
                      return left.DrawOrder > right.DrawOrder;
                  });

        return result;
    }

    GenderType Character::gender() const
    {
        return mHistory->gender();
    }

    bool Character::isSuffocating() const
    {
        return mBody->isSuffocating();
    }

    bool Character::findDryLand()
    {
        return mPathfinder->findDryLand(position());
    }

    std::shared_ptr<Character> Character::currentTarget() const
    {
        return mCurrentTarget;
    }

    bool Character::shouldRunFromTarget() const
    {
        return shouldRunFromTarget(mCurrentTarget);
    }

    bool Character::shouldRunFromTarget(std::shared_ptr<const Character> target) const
    {
        if (target==nullptr)
            return (mRecentlyRanTimer>0.0f);

        if (target->body()== nullptr)
            return (mRecentlyRanTimer>0.0f);

        if ((!mTrackEnemy) && (!isAttackTarget(target)))
        {
            const auto leaderTarget=squadLeaderTarget();
            if ((leaderTarget!= nullptr) && (target->ID()!=leaderTarget->ID()))
            {
                const auto delta=target->position()-position();
                if (delta.length()>combatSightRadius())
                    return false;
            }
        }

        return isAfraid(target);
    }

    bool Character::isAttackTarget(std::shared_ptr<const Character> target) const
    {
        if (mSquad==nullptr)
            return false;

        if (mSquad->formation()==nullptr)
            return false;

        return mSquad->isAttackTarget(std::move(target));
    }

    std::shared_ptr<Character> Character::squadLeaderTarget() const
    {
        const auto pos=squadPosition();
        if (pos== nullptr)
            return nullptr;

        if (!pos->attackSquadLeaderTarget())
            return nullptr;

        if (mSquad->squadLeader()->ID()==ID())
            return nullptr;

        return mSquad->squadLeaderTarget();
    }

    int Character::combatSightRadius() const
    {
        if ((mSquad!=nullptr) && (mSquad->formation()!=nullptr) && (mSquad->formation()->avoidEnemies()))
            return 2;

        if ((mSquad==nullptr) && (mHistory->factionDef()->Type==FactionType::PlayerCiv))
            return 2;

        return int(mHistory->raceDef()->SightRadius);
    }

    bool Character::isAfraid(std::shared_ptr<const Character> target) const
    {
        const auto squad_pos=squadPosition();
        if ((squad_pos!= nullptr) && (squad_pos->perk()==SquadPositionPerk::WayOfTheGnome))
            return false;

        const float combat=2.0f*(combatValue()+mMind->combatValue());
        return (combat<target->combatValue());
    }

    float Character::combatValue() const
    {
        const float baseCombarValue=(mHistory->raceClassDef()!= nullptr) ? mHistory->raceClassDef()->BaseCombatValue : 1.0f;
        return baseCombarValue+mBody->combatValue();
    }

    std::shared_ptr<const Squad> Character::squad() const
    {
        return mSquad;
    }

    std::shared_ptr<const properties::FactionDef> Character::factionDef() const
    {
        return mHistory->factionDef();
    }

    bool Character::canPickupWeapon() const
    {
        return mBody->canPickupWeapon();
    }

    std::vector<std::shared_ptr<Weapon>> Character::naturalWeapons() const
    {
        return mBody->naturalWeapons();
    }

    int Character::skillLevel(const std::string& id) const
    {
        return mHistory->skillLevel(id);
    }

    bool Character::hasAmmo(std::shared_ptr<const Weapon> weapon) const
    {
        return mBody->hasAmmo(std::move(weapon));
    }

    bool Character::withinTargetRange() const
    {
        if (mCurrentTarget==nullptr)
            return false;

        const auto delta_pos=position()-mCurrentTarget->position();
        return mBody->hasWeaponInRange(delta_pos.length());
    }

    bool Character::shouldMoveToMaxRange() const
    {
        if (mCurrentTarget==nullptr)
            return false;

        const auto squad_pos=squadPosition();
        if (squad_pos==nullptr)
            return false;

        if (!squad_pos->stayAtMaxRange())
            return false;

        if (mSquad==nullptr)
            return false;

        if (!mSquad->isInMembers(mCurrentTarget))
            return false;

        const auto delta_pos=position()-mCurrentTarget->position();
        if (delta_pos.length()>(mBody->maxRange()-2.0f))
            return false;

        return true;
    }

    bool Character::isRetreatingToHeal() const
    {
        if (!mBody->isBleeding())
            return false;

        const auto squad_pos=squadPosition();
        if (squad_pos==nullptr)
            return false;

        if (!squad_pos->retreatWhenBleeding())
            return false;

        if (mCurrentNeedGoal==nullptr)
            return false;

        if (mCurrentNeedGoal->effectAmount(ItemEffectType::Heal)==0.0f)
            return false;

        return true;
    }

    bool Character::isRetreatingToEquip() const
    {
        const auto squad_pos=squadPosition();

        if (squad_pos==nullptr)
            return false;

        if (!squad_pos->retreatToEquip())
            return false;

        if (mCurrentNeedGoal==nullptr)
            return false;

        const auto goal=std::dynamic_pointer_cast<Item>(mCurrentNeedGoal);
        if (goal==nullptr)
            return false;

        const auto itemID=goal->itemID();
        if (UNIFORM_SETTINGS->isArmor(itemID))
            return true;

        if (UNIFORM_SETTINGS->isWeapon(itemID))
            return true;

        return false;
    }

    bool Character::isRetreatingToRefillAmmo() const
    {
        const auto squad_pos=squadPosition();

        if (squad_pos==nullptr)
            return false;

        if (!squad_pos->retreatToFillAmmo())
            return false;

        if (mCurrentNeedGoal==nullptr)
            return false;

        const auto goal=std::dynamic_pointer_cast<Item>(mCurrentNeedGoal);
        if (goal==nullptr)
            return false;

        const auto itemID=goal->itemID();
        return (ITEM_SETTINGS->itemIDToAmmoID(itemID)!=AMMOID_NONE);
    }

    bool Character::shouldStopRunning() const
    {
        if (!withinTargetRange())
            return false;

        if (shouldMoveToMaxRange())
            return false;

        if (isRetreatingToHeal())
            return false;

        if (isRetreatingToEquip())
            return false;

        if (isRetreatingToRefillAmmo())
            return false;

        return true;
    }

    bool Character::runAwayFromTarget()
    {
        if (mCurrentTarget==nullptr)
        {
            if ((withinTargetRange()) || (mRecentlyRanTimer<=0.0f))
                return false;
        }
        else
        {
            if ((mRecentlyRanTimer<=0.0f) && (isAfraid(mCurrentTarget)))
                mMind->adjustHappiness(-10.0f);

            mRecentlyRanTimer=5.0f;
        }

        if (mJob!=nullptr)
            cancelJob("");

        const auto stairs=cell()->stairs();
        if (stairs!=nullptr)
        {
            if (stairs->position()==position())
                return pathTo(position()+map::vector3(0,0,-1),false);

            return pathTo(position()+map::vector3(0,0,1),false);
        }

        map::vector3 target_pos=(mCurrentTarget!= nullptr) ? mCurrentTarget->position() : mLastEnemyLocation;
        const auto ramp=cell()->ramp();

        std::vector<map::vector3> deltas(10);
        deltas[0]=map::vector3(-1,-1,0);
        deltas[1]=map::vector3(0,-1,0);
        deltas[2]=map::vector3(1,-1,0);
        deltas[3]=map::vector3(-1,0,0);
        deltas[4]=map::vector3(1,0,0);
        deltas[5]=map::vector3(-1,1,0);
        deltas[6]=map::vector3(0,1,0);
        deltas[7]=map::vector3(1,1,0);

        map::vector3 new_pos=position();
        map::vector3 delta_pos=target_pos-new_pos;
        float max_distance=delta_pos.length();
        for (const auto& delta : deltas)
        {
            map::vector3 pos=position()+delta;

            if (ramp!=nullptr)
            {
                map::vector3 temp = pos + map::vector3(0, 0, -1);
                const auto cell=WORLD_MAP->cell(temp);
                if ((cell!= nullptr) && (cell->isWalkable()))
                {
                    delta_pos=target_pos-temp;
                    float distance=delta_pos.length();
                    if (distance>max_distance)
                    {
                        max_distance=distance;
                        new_pos=temp;
                    }
                }
            }

            {
                map::vector3 temp = pos + map::vector3(0, 0, 1);
                const auto cell=WORLD_MAP->cell(temp);
                if ((cell!= nullptr) && (cell->hasRamp()))
                {
                    delta_pos=target_pos-temp;
                    float distance=delta_pos.length();
                    if (distance>max_distance)
                    {
                        max_distance=distance;
                        new_pos=temp;
                    }
                }
            }

            delta_pos=target_pos-pos;
            const auto cell=WORLD_MAP->cell(delta_pos);
            if ((cell!= nullptr) && (cell->isWalkable()))
            {
                float distance = delta_pos.length();
                if (distance > max_distance)
                {
                    max_distance = distance;
                    new_pos = pos;
                }
            }
        }

        pathTo(new_pos,false);
    }

    std::shared_ptr<Body> Character::body() const
    {
        return mBody;
    }

    std::shared_ptr<Mind> Character::mind() const
    {
        return mMind;
    }

    bool Character::moveToMaxRange()
    {
        const auto stairs=cell()->stairs();
        if (stairs!=nullptr)
        {
            if (stairs->position()==position())
                return pathTo(position()+map::vector3(0,0,-1),false);

            return pathTo(position()+map::vector3(0,0,1),false);
        }

        map::vector3 target_pos=(mCurrentTarget!= nullptr) ? mCurrentTarget->position() : mLastEnemyLocation;
        const auto ramp=cell()->ramp();
        float max_range=mBody->maxRange();

        std::vector<map::vector3> deltas(10);
        deltas[0]=map::vector3(-1,-1,0);
        deltas[1]=map::vector3(0,-1,0);
        deltas[2]=map::vector3(1,-1,0);
        deltas[3]=map::vector3(-1,0,0);
        deltas[4]=map::vector3(1,0,0);
        deltas[5]=map::vector3(-1,1,0);
        deltas[6]=map::vector3(0,1,0);
        deltas[7]=map::vector3(1,1,0);

        map::vector3 delta_pos=position()-target_pos;
        float current_range=delta_pos.length();
        std::vector<map::vector3> new_pos;
        for (const auto& delta : deltas)
        {
            map::vector3 pos=position()+delta;

            if (ramp!=nullptr)
            {
                map::vector3 temp = pos + map::vector3(0, 0, -1);
                const auto cell=WORLD_MAP->cell(temp);
                if ((cell!= nullptr) && (cell->isWalkable()))
                {
                    delta_pos=target_pos-temp;
                    float distance=delta_pos.length();
                    if ((distance>current_range) && (current_range<=max_range))
                    {
                        new_pos.push_back(temp);
                    }
                }
            }

            {
                map::vector3 temp = pos + map::vector3(0, 0, 1);
                const auto cell=WORLD_MAP->cell(temp);
                if ((cell!= nullptr) && (cell->hasRamp()))
                {
                    delta_pos=target_pos-temp;
                    float distance=delta_pos.length();
                    if ((distance>current_range) && (current_range<=max_range))
                    {
                        new_pos.push_back(temp);
                    }
                }
            }

            delta_pos=target_pos-pos;
            const auto cell=WORLD_MAP->cell(delta_pos);
            if ((cell!=nullptr) && (cell->isWalkable()))
            {
                float distance = delta_pos.length();
                if ((distance > current_range) && (current_range <= max_range))
                {
                    new_pos.push_back(delta_pos);
                }
            }
        }

        if (!new_pos.empty())
        {
            const auto index=(size_t)RANDOMIZER->rand(0,(int)new_pos.size()-1);
            const auto pos_to_move=new_pos.at(index);
            return pathTo(pos_to_move,false);
        }

        return false;
    }

    float Character::adjustDamageByPerks(std::shared_ptr<const Weapon> weapon, std::shared_ptr<const properties::AttackDef> attackDef) const
    {
        const auto squad_pos=squadPosition();
        if (squad_pos==nullptr)
            return 1.0f;

        float value=1.0f;

        const auto perk=squad_pos->perk();
        if (perk==SquadPositionPerk::Highlander)
        {
            if (weapon->item()!=nullptr)
            {
                if ((GAME_DEFINITIONS->itemDefinition(weapon->item()->itemID())->TwoHanded) && (mBody->equippedItemsCount()==0))
                    value=1.4f;
            }
        }
        else if (perk==SquadPositionPerk::WayOfTheGnome)
        {
            if ((mBody->equippedItemsCount()==0) && (mBody->heldItemsCount()==0))
            {
                const auto skill=NATURALATTACKSKILLID;
                value=1.0f+skillLevel(skill)/25.0f;
            }
        }
        else if (perk==SquadPositionPerk::Sharpshooter)
        {
            if ((mCurrentTarget!= nullptr) && (attackDef->isRangedAttack()))
            {
                const auto delta_pos=mCurrentTarget->position()-position();
                const float distance=delta_pos.length();
                const float range=attackDef->AttackRange*0.5f;
                if (distance>range)
                    value=1.0f+(distance-range)/(range*0.200000002980232f);
            }
        }
        else if (perk==SquadPositionPerk::FinishHim)
        {
            if ((mCurrentTarget!= nullptr) && (mCurrentTarget->body()->containStatusEffect(HealthStatusAilment::FallenOver)))
                value=1.2f;
        }
        else if (perk==SquadPositionPerk::Sneaky)
        {
            if ((mCurrentTarget->currentTarget()->ID()!=ID()) && (isSneaking()))
                value=1.2f;
        }
        else if (perk==SquadPositionPerk::Swashbuckler)
        {
            if (weapon->item()!=nullptr)
            {
                const auto itemID=weapon->item()->itemID();
                const auto pistolID=PISTOLID;
                const auto sworldID=SWORDID;
                const auto items=heldItems();
                if (items.size()==2)
                {
                    const auto firstItemID=items.at(0)->itemID();
                    const auto secondItemID=items.at(1)->itemID();
                    if (((firstItemID==pistolID) && (secondItemID==sworldID)) || ((firstItemID==sworldID) && (secondItemID==pistolID)))
                    {
                        const auto delta_pos=mCurrentTarget->position()-position();
                        const auto distance=delta_pos.length();
                        if (distance<2.0)
                            value=1.2f;
                    }
                }
            }
        }

        value*=mSquad->commanderDamageBonus(std::dynamic_pointer_cast<const Character>(shared_from_this()));
        float formationBonus;
        if (mBody->maxRange()<=2.0f)
            formationBonus=mSquad->formationMeleeDamageBonus();
        else
            formationBonus=mSquad->formationRangedDamageBonus();

        return value*formationBonus;
    }

    bool Character::isSneaking() const
    {
        const auto squad_pos=squadPosition();
        if (squad_pos==nullptr)
            return false;

        if (squad_pos->perk()!=SquadPositionPerk::Sneaky)
            return false;

        if (mBody->maxRange()>2.0f)
            return false;

        const auto equippedItems=mBody->equippedItems();
        for (const auto& item : equippedItems)
        {
            const auto materialID=item->materialID();
            if (GAME_DEFINITIONS->materialDefinition(materialID)->Type==MaterialType::Metal)
                return false;
        }

        return true;
    }

    void Character::increaseAttribute(const CharacterAttributeType& type, const float& value)
    {
        if (!mHistory->increaseAttribute(type,value))
            return;

        mMind->adjustHappiness(5.0f*mHistory->attributeDeltaLevel(type));
    }

    void Character::increaseSquadmatesAttribute(const CharacterAttributeType& type, const float& value)
    {
        if (mSquad==nullptr)
            return;

        const auto members=mSquad->members();
        for (const auto& member : members)
        {
            if (member==nullptr)
                continue;

            if (member->ID()==ID())
                continue;

            if (member->ID()==mCurrentTarget->ID())
                continue;

            if (std::dynamic_pointer_cast<TrainOrder>(member->job())==nullptr)
                continue;

            member->increaseAttribute(type,value);
        }
    }

    float Character::attackMissRate(const std::string& weaponSkillID) const
    {
        if (mBody->containStatusEffect(HealthStatusAilment::Blind))
            return 0.9f;

        return std::max(0.01f,1.0f-(mHistory->skillModifier(weaponSkillID)*attributeLevel(CharacterAttributeType::Focus)
        *0.100000001490116f+0.899999976158142f));
    }

    void Character::increaseSkill(const std::string& id, const float& value)
    {
        if (!mHistory->increaseSkill(id,value))
            return;

        mMind->adjustHappiness(0.5f*float(mHistory->skillLevel(id)));
    }

    void Character::increaseSquadmatesSkill(const std::string& id, const float& value, bool increaseEquippedWeapon)
    {
        if (mSquad==nullptr)
            return;

        const auto members=mSquad->members();
        for (const auto& member : members)
        {
            if (member==nullptr)
                continue;

            if (member->ID()==ID())
                continue;

            if (std::dynamic_pointer_cast<TrainOrder>(member->job())==nullptr)
                continue;

            member->increaseSkill(id,value);

            if (increaseEquippedWeapon)
            {
                const auto items=member->body()->heldItems();
                for (const auto& item : items)
                {
                    const std::string skillID=GAME_DEFINITIONS->itemDefinition(item->itemID())->ItemWeaponDef->Skill;
                    member->increaseSkill(skillID,value);
                }
            }
        }
    }

    bool Character::hasUniqueName() const
    {
        return mHistory->hasUniqueName();
    }

    std::string Character::name() const
    {
        return mHistory->name();
    }

    float Character::formationBlockChanceBonus() const
    {
        if (mSquad==nullptr)
            return 1.0f;

        return mSquad->formationBlockChanceBonus();
    }

    float Character::formationBlockRateBonus() const
    {
        if (mSquad==nullptr)
            return 1.0f;

        return mSquad->formationBlockRateBonus();
    }

    bool Character::defendAttack(const float& attackerWeaponSkill, std::shared_ptr<Attack> attack, std::shared_ptr<BodySection> sectionHit,
                      std::vector<std::string>& log_string)
    {
        float randEffect=RANDOMIZER->uniform(0.0f,1.0f);

        if ((mBody->containStatusEffect(HealthStatusAilment::Unconcious)) && (randEffect<0.949999988079071f))
            return false;

        if ((mBody->isSleeping()) && (randEffect<0.949999988079071f))
            return false;

        if ((mBody->containStatusEffect(HealthStatusAilment::Blind)) && (randEffect<0.949999988079071f))
            return false;

        bool isTraining=false;
        if ((mSquad!=nullptr) && (mSquad->isInMembers(mCurrentTarget)))
            isTraining=true;

        std::vector<std::pair<std::string,float>> skills;
        skills.push_back({CHARACTER_SETTINGS->DodgeSkill,float(skillLevel(CHARACTER_SETTINGS->DodgeSkill))});

        const auto weapon_list=mBody->defenseWeapons();
        const auto attackType=attack->attackDef()->AttackType;
        for (const auto& weapon : weapon_list)
        {
            const auto def_list=weapon->weaponDef()->DefendMoves;
            const std::string skill=weapon->weaponDef()->Skill;

            for (const auto& def_elem : def_list)
            {
                auto iter=std::find_if(skills.begin(),skills.end(),[&skill](std::pair<std::string,float> const& elem)
                {
                    return elem.first==skill;
                });

                if (iter!=skills.end())
                    continue;

                if (def_elem->defendsAgainst(attackType))
                    skills.push_back({skill,float(skillLevel(skill))});
            }
        }

        std::sort(skills.begin(), skills.end(),
                  [](const std::pair<std::string,float> & left, const std::pair<std::string,float> & right) -> bool
                  {
                      return left.second < right.second;
                  });

        for (const auto& skill_pair : skills)
        {
            const auto skill=skill_pair.first;
            float value=float(skillLevel(skill))*attributeLevel(CharacterAttributeType::Nimbleness);
            float def_modifier=1.0f;
            const auto squad_pos=squadPosition();
            if ((squad_pos!=nullptr) && (squad_pos->perk()==SquadPositionPerk::Frenzy))
            {
                const auto heldItems=mBody->heldItems();
                if (heldItems.size()==2)
                {
                    const auto firstItem=heldItems.at(0);
                    const auto secondItem=heldItems.at(1);
                    if ((!UNIFORM_SETTINGS->isShield(firstItem->itemID()))
                        && (!UNIFORM_SETTINGS->isShield(secondItem->itemID())))
                        value*=0.5f;
                }
                else if (heldItems.size()==1)
                {
                    const auto firstItem=heldItems.at(0);
                    if (!UNIFORM_SETTINGS->isShield(firstItem->itemID()))
                        value*=0.5f;
                }
            }

            if (skill==CHARACTER_SETTINGS->DodgeSkill)
            {
                if (attack->attackDef()->RequiresAmmo)
                {
                    const auto ammoItemID=attack->weapon()->weaponDef()->AmmoItemID;
                    const auto itemID=ITEM_SETTINGS->itemIDToAmmoID(ammoItemID);
                    const auto modifier=GAME_DEFINITIONS->ammoDefinition(itemID)->VelocityModifier;
                    value/=modifier;
                }

                value*=mBody->movementPenalty();

                if (squad_pos!=nullptr)
                {
                    if ((squad_pos->perk()==SquadPositionPerk::WayOfTheGnome) && (mBody->equippedItemsCount()==0)
                        && (mBody->heldItemsCount()==0))
                    {
                        float modifier=1.0f+float(skillLevel(skill))/200.0f;
                        value*=modifier;
                    }
                    else
                    {
                        value*=1.4f;
                    }
                }
            }
            else
            {
                const auto shieldSkill=SHIELDSKILLID;

                if (squad_pos!=nullptr)
                {
                    const auto perk=squad_pos->perk();
                    if (perk==SquadPositionPerk::Guard)
                    {
                        if (skill==shieldSkill)
                        {
                            const auto heldItems=mBody->heldItems();
                            for (const auto& item : heldItems)
                            {
                                if (UNIFORM_SETTINGS->isShield(item->itemID()))
                                {
                                    const float modifier=1.0f+float(skillLevel(shieldSkill))/200.0f;
                                    value*=modifier;
                                }
                            }
                        }
                    }
                    else if (perk==SquadPositionPerk::Swashbuckler)
                    {
                        const auto sworldSkill=SWORDSKILLID;
                        if ((skill==sworldSkill) && (mBody->heldItemsCount()==2))
                        {
                            const auto heldItems=mBody->heldItems();
                            const auto firstItemID=heldItems.at(0)->itemID();
                            const auto secondItemID=heldItems.at(1)->itemID();
                            const auto pistolItemID=PISTOLID;
                            const auto swordItemID=SWORDID;
                            if (((firstItemID==pistolItemID) && (secondItemID==swordItemID))
                                || ((firstItemID==swordItemID) && (secondItemID==pistolItemID)))
                            {
                                const float modifier=1.0f+float(skillLevel(sworldSkill))/200.0f;
                                value*=modifier;
                            }
                        }
                    }
                }

                if (skill==shieldSkill)
                {
                    value*=formationBlockChanceBonus();
                    def_modifier*=formationBlockRateBonus();
                }
            }

            const float randAttack=RANDOMIZER->uniform(.0f,1.0f);
            if (randAttack<(value/2.0f/attackerWeaponSkill))
            {
                if (skill==CHARACTER_SETTINGS->DodgeSkill)
                {
                    if ((mBody->canDodge()) && (!mBody->containStatusEffect(HealthStatusAilment::FallenOver)))
                    {
                        std::vector<map::vector3> deltas(8);
                        deltas[0]=map::vector3(-1,-1,0);
                        deltas[1]=map::vector3(0,-1,0);
                        deltas[2]=map::vector3(1,-1,0);
                        deltas[3]=map::vector3(-1,0,0);
                        deltas[4]=map::vector3(1,0,0);
                        deltas[5]=map::vector3(-1,1,0);
                        deltas[6]=map::vector3(0,1,0);
                        deltas[7]=map::vector3(1,1,0);

                        std::vector<map::vector3> pos_candidates;
                        for (const auto& delta : deltas)
                        {
                            const auto pos=position()+delta;
                            const auto cell=WORLD_MAP->cell(pos);
                            if ((cell!=nullptr) && (cell->getCharactersCount()==0) && (cell->isWalkable()))
                                pos_candidates.push_back(pos);
                        }

                        if (pos_candidates.empty())
                        {
                            const auto rand=RANDOMIZER->rand(0,1);
                            if (rand==0)
                                continue;
                        }
                        else
                        {
                            mBody->dodged();
                            const auto rand=(size_t)RANDOMIZER->rand(0,(int)pos_candidates.size()-1);
                            moveTo(pos_candidates.at(rand));
                        }

                        if (isTraining)
                        {
                            if (value<attackerWeaponSkill)
                                increaseSkill(skill,1.0f);
                            else
                                increaseSkill(skill,0.3f);

                            increaseSquadmatesSkill(skill,0.15f,false);
                        }
                        else
                        {
                            increaseSkill(skill,5.0f);
                        }

                        increaseAttribute(CharacterAttributeType::Nimbleness,isTraining ? 0.1f:1.0f);
                        log_string.emplace_back("dodges");
                        return true;
                    }
                }
                else
                {
                    std::vector<std::shared_ptr<Weapon>> weapon_list_with_skill;
                    for (const auto& weapon : weapon_list)
                    {
                        if ((weapon->weaponDef()->Skill==skill) && (!weapon->weaponDef()->DefendMoves.empty()))
                            weapon_list_with_skill.push_back(weapon);
                    }

                    if (!weapon_list_with_skill.empty())
                    {
                        const auto rand_weapon_index=(size_t)RANDOMIZER->rand(0,(int)weapon_list_with_skill.size()-1);
                        const auto weapon=weapon_list_with_skill.at(rand_weapon_index);

                        const auto defends=weapon->weaponDef()->defendMoves(attack->attackDef()->AttackType);
                        const auto rand_def_index=(size_t)RANDOMIZER->rand(0,(int)defends.size()-1);
                        const auto defendDef=defends.at(rand_def_index);
                        weapon->defend(defendDef,def_modifier);

                        if (attackType==AttackType::Thrust)
                            increaseAttribute(CharacterAttributeType::Nimbleness,isTraining ? 0.1f:1.0f);
                        else if ((attackType==AttackType::Swing) || (attackType==AttackType::HandToHand)
                            || (attackType==AttackType::Projectile))
                            increaseAttribute(CharacterAttributeType::Fitness,isTraining ? 0.1f:1.0f);

                        if (isTraining)
                        {
                            if (value<attackerWeaponSkill)
                                increaseSkill(skill,1.0f);
                            else
                                increaseSkill(skill,0.3f);

                            increaseSquadmatesSkill(skill,0.15f,false);
                        }
                        else
                        {
                            increaseSkill(skill,5.0f);
                        }

                        log_string.push_back(defendDef->Verb);

                        if ((weapon->item()!=nullptr) && (weapon->item()->hasUniqueName()))
                            log_string.push_back("with "+weapon->name());
                        else
                            log_string.push_back("with "+mHistory->possessivePronoun()+" "+weapon->name());
                    }
                }
            }
        }

        return false;
    }

    void Character::setLastAttacker(std::shared_ptr<CharacterHistory> lastAttacker)
    {
        mLastAttacker=std::move(lastAttacker);
    }

    void Character::lostLimb(std::shared_ptr<BodySection> section)
    {
        map::vector3 groundPosDrink1(45,40,20);
        AIDIRECTOR->createBodyPartItem(position(),std::dynamic_pointer_cast<Character>(shared_from_this()),
                section,ITEM_SETTINGS->LimbItemID);
    }

    void Character::finishDroppingItems(std::shared_ptr<Character> character)
    {
        const auto itemsToDrop=character->body()->itemsToDrop();

        if (itemsToDrop.empty())
            return;

        std::string log;

        if (!character->hasUniqueName())
            log+="The ";

        log+=character->name()+" drops";

        if (itemsToDrop.front()->hasUniqueName())
            log+=itemsToDrop.front()->name();
        else
            log+="the "+itemsToDrop.front()->name();

        character->dropItem(itemsToDrop.front());

        for (size_t i=1,isize=itemsToDrop.size();i<isize;i++)
        {
            if (i<isize-1)
                log+=", ";
            else
                log+=" and ";

            if (!itemsToDrop.at(i)->hasUniqueName())
                log+="the ";

            log+=itemsToDrop.at(i)->name();

            character->dropItem(itemsToDrop.at(i));
        }

        std::cout << log << std::endl;
    }

    void Character::killed()
    {
        const auto faction=AIDIRECTOR->faction(mHistory->factionID());
        if ((faction!=nullptr) && (!faction->isDeceasedMembersContain(ID())))
        {
            const auto character=std::dynamic_pointer_cast<Character>(shared_from_this());

            AIDIRECTOR->createBodyPartItem(position(),character,nullptr,ITEM_SETTINGS->CorpseItemID);
            faction->memberDied(character);
            leaveCombat();
            GMINSTANCE->addToDeleteList(shared_from_this());
        }
    }

    void Character::leaveCombat()
    {
        mCurrentTarget=nullptr;
        mBody->repairEquipment();
    }

    void Character::attackTarget(const float& dt)
    {
        if (mCurrentTarget==nullptr)
            return;

        if (mCurrentTarget->body()==nullptr)
            return;

        if (mCurrentTarget->body()->isDead())
            return;

        const float distance=map::vector3::distance(position(),mCurrentTarget->position());

        const auto weapon_list=mBody->weapons(distance);
        if (weapon_list.empty())
            return;

        const auto rand_index=(size_t)RANDOMIZER->rand(0,(int)weapon_list.size()-1);
        const auto rand_weapon=weapon_list.at(rand_index);

        for (size_t i=0,isize=weapon_list.size();i<isize;i++)
        {
            if (i!=rand_index)
            {
                weapon_list.at(i)->delayAttack(raceDefinition()->BaseAttackDelay);
            }
        }

        const std::string rand_weapon_skill_id=rand_weapon->weaponDef()->Skill;

        float fitness=attributeLevel(CharacterAttributeType::Fitness);
        if (mMind->isUpset())
            fitness*=1.5f;

        bool isTraining=false;
        if ((mSquad!= nullptr) && (mSquad->isInMembers(mCurrentTarget)))
        {
            isTraining=true;
            fitness=0.0f;
        }

        const auto sectionHit=mCurrentTarget->body()->randomSection(std::dynamic_pointer_cast<Character>(shared_from_this()));

        float velocity=attributeLevel(CharacterAttributeType::Nimbleness);

        std::shared_ptr<properties::AttackDef> attackDef=rand_weapon->bestAttackInRange(distance,
                std::dynamic_pointer_cast<Character>(shared_from_this()),sectionHit);

        float baseDamage=fitness*adjustDamageByPerks(rand_weapon,attackDef);
        const auto attack=std::make_shared<Attack>(rand_weapon,attackDef,baseDamage,velocity);

        float attackTime=attackDef->AttackTime/3.0f;
        if (rand_weapon->item()!= nullptr)
        {
            const auto itemID=rand_weapon->item()->itemID();
            if (GAME_DEFINITIONS->itemDefinition(itemID)->TwoHanded)
                attackTime*=2.0f;
        }

        const auto squad_pos=squadPosition();
        rand_weapon->attack(attackDef);
        std::shared_ptr<Item> ammoItem=nullptr;
        if (attackDef->RequiresAmmo)
        {
            bool toSuspendAmmo=true;
            if (isTraining)
                toSuspendAmmo=false;
            else
            {
                if (squad_pos != nullptr)
                {
                    if (squad_pos->perk() == SquadPositionPerk::Conservationist)
                        toSuspendAmmo = (RANDOMIZER->rand(0, 1) == 0);
                }
            }

            ammoItem=mBody->consumeAmmo(rand_weapon->weaponDef()->AmmoItemID,toSuspendAmmo);
        }

        increaseAttribute(CharacterAttributeType::Fitness,(isTraining ? 0.1f : 1.0f)*attackTime);

        std::string log_str;
        if (mHistory->hasUniqueName())
            log_str+="The ";

        log_str+=mHistory->name()+" ";
        log_str+=attackDef->Verb+" ";

        std::string add_str="with ";
        if (ammoItem!=nullptr)
        {
            const std::string ammoName=ammoItem->name();
            log_str+="a "+ammoName+" ";
            add_str="from ";
        }

        const std::string weapon_name=rand_weapon->name();
        if (!weapon_name.empty())
        {
            if ((rand_weapon->item()!=nullptr) && (rand_weapon->item()->hasUniqueName()))
                log_str+=add_str+weapon_name+" ";
            else
                log_str+=add_str+mHistory->possessivePronoun()+" "+weapon_name+" ";
        }

        if (mCurrentTarget->hasUniqueName())
            log_str+="the ";

        log_str+=mCurrentTarget->name()+" ";

        float missRate=1.0f;
        if (squad_pos!= nullptr)
        {
            if ((squad_pos->perk()==SquadPositionPerk::Marksgnome) && (attackDef->AttackRange>2.0f))
                missRate*=0.8f;
        }

        const float rand=RANDOMIZER->uniform(0.0f,1.0f);
        if (rand>attackMissRate(rand_weapon_skill_id)*missRate)
        {
            increaseAttribute(CharacterAttributeType::Focus,(isTraining ? 0.1f : 1.0f)*attackTime);

            if (isTraining)
                increaseSquadmatesAttribute(CharacterAttributeType::Focus,0.05f*attackTime);

            if (isTraining)
            {
                if (skillLevel(rand_weapon_skill_id)<mCurrentTarget->skillLevel(rand_weapon_skill_id))
                    increaseSkill(rand_weapon_skill_id,1.0f*attackTime);
                else
                    increaseSkill(rand_weapon_skill_id,0.3f*attackTime);

                increaseSquadmatesSkill(rand_weapon_skill_id,0.15f*attackTime,true);
            }
            else
            {
                increaseSkill(rand_weapon_skill_id,5.0f*attackTime);
            }

            std::vector<std::string> attackString;
            std::vector<std::string> statusEffects;

            float attackerWeaponSkill=float(skillLevel(rand_weapon->weaponDef()->Skill))*attributeLevel(CharacterAttributeType::Nimbleness)
                    *rand_weapon->weaponDef()->VelocityModifier;
            if ((mSquad!= nullptr) && (attackDef->AttackRange<=2.0f))
                attackerWeaponSkill*=mSquad->formationAttackSkillBonus();

            if (mCurrentTarget->defendAttack(attackerWeaponSkill,attack,sectionHit,attackString))
            {
                log_str+=" but ";
                if (!mCurrentTarget->hasUniqueName())
                    log_str+="the ";

                log_str+=mCurrentTarget->name()+" "+attackString.at(0)+" the attack";

                if (attackString.size()>1)
                    log_str+=" "+attackString.at(1);

                log_str+="!";

                std::cout << log_str << std::endl;
            }
            else
            {
                mCurrentTarget->setLastAttacker(mHistory);
                mCurrentTarget->body()->damage(attack,sectionHit,attackString,statusEffects);
                mCurrentTarget->increaseAttribute(CharacterAttributeType::Fitness,(isTraining ? 0.1f : 1.0f)*attackTime);
                bool isDead=mCurrentTarget->body()->isDead();
                if (!isDead)
                {
                    const auto randKnockback=RANDOMIZER->uniform(0.0f,1.0f);
                    if (randKnockback<rand_weapon->weaponDef()->KnockbackModifier*0.100000001490116f)
                    {
                        const auto pos=mCurrentTarget->position()+(mCurrentTarget->position()-position());
                        const auto cell=WORLD_MAP->cell(pos);
                        if ((cell!=nullptr) && (!cell->blocksMovement()))
                        {
                            statusEffects.emplace_back("is knocked back.");
                            mCurrentTarget->moveTo(pos,true);
                        }
                    }
                }

                std::cout << log_str << std::endl;
                log_str.clear();

                finishDroppingItems(mCurrentTarget);

                if (!isDead)
                    return;

                if (!mCurrentTarget->hasUniqueName())
                    log_str+="The ";

                log_str+=mCurrentTarget->name()+" has died.";
                std::cout << log_str << std::endl;

                mMind->adjustHappiness(20.0f);
                mHistory->killed(mCurrentTarget);

                if (rand_weapon->item()!=nullptr)
                    rand_weapon->item()->history()->killed(mCurrentTarget);

                mCurrentTarget->killed();

                leaveCombat();
                mTrackEnemy=false;
            }
        }
        else
        {
            if (!isTraining)
                increaseSkill(rand_weapon_skill_id,0.1f*attackTime);

            log_str+=" and misses!";
            std::cout << log_str << std::endl;
        }
    }

    map::vector3 Character::lastEnemyLocation() const
    {
        return mLastEnemyLocation;
    }

    void Character::setTrackEnemy(const bool& value)
    {
        mTrackEnemy=value;
    }

    bool Character::trackEnemy() const
    {
        return mTrackEnemy;
    }

    float Character::formationVisionBonus() const
    {
        if (mSquad==nullptr)
            return 1.0f;

        return mSquad->formationVisionBonus();
    }

    int Character::castLightRadius() const
    {
        float value=raceDefinition()->CastLightRadius;

        const auto heldItems=mBody->heldItems();
        for (const auto& item : heldItems)
        {
            float effect=item->effectAmount(ItemEffectType::LightSource);
            if (effect>value)
            {
                const auto squad_pos=squadPosition();
                if ((squad_pos!=nullptr) && (squad_pos->perk()==SquadPositionPerk::Scout))
                    effect*=1.25f;

                value=effect;
            }
        }

        return (int)std::floor(value*formationVisionBonus());
    }

    void Character::clearVisibleList()
    {
        for (const auto& visibleCell : mVisibleCells)
        {
            const auto visibility=visibleCell.second->visibility();
            visibleCell.second->setVisibility(visibility-visibleCell.first);
        }

        mVisibleCells.clear();
    }

    void Character::castLight(const int& radius)
    {
        //TODO
    }

    bool Character::isOffMap() const
    {
        //TODO
        return false;
    }

    float Character::minimumVisibleDistance() const
    {
        return (isSneaking()) ? 2.0f : FLT_MAX;
    }

    bool Character::hasLOS(const map::vector3& start,const map::vector3& end) const
    {
        if (start==end)
            return true;

        int x1=start.x();
        int y1=start.y();
        int z1=start.z();
        int x2=end.x();
        int y2=end.y();
        int z2=end.z();
        int dx=x2-x1;
        int dy=y2-y1;
        int dz=z2-z1;

        int shiftX,shiftY,shiftZ;
        float x0,stepX,y0,stepY,z0,stepZ;

        if (dx==0)
        {
            shiftX=0;
            x0=FLT_MAX;
            stepX=0.0f;
        }
        else
        {
            shiftX=(dx<0) ? -1 : 1;
            x0=0.5f/float(dx);
            stepX=1.0f/float(dx);
        }

        if (dy==0)
        {
            shiftY=0;
            y0=FLT_MAX;
            stepY=0.0f;
        }
        else
        {
            shiftY=(dy<0) ? -1 : 1;
            y0=0.5f/float(dy);
            stepY=1.0f/float(dy);
        }

        if (dz==0)
        {
            shiftZ=0;
            z0=FLT_MAX;
            stepZ=0.0f;
        }
        else
        {
            shiftZ=(dz<0) ? -1 : 1;
            z0=0.5f/float(dz);
            stepZ=1.0f/float(dz);
        }

        x0=std::abs(x0);
        stepX=std::abs(stepX);
        y0=std::abs(y0);
        stepY=std::abs(stepY);
        z0=std::abs(z0);
        stepZ=std::abs(stepZ);

        while ((((x1<=x2) && (shiftX>0)) || ((x1>=x2) && (shiftX<0)) || (shiftX==0))
            && (((y1<=y2) && (shiftY>0)) || ((y1>=y2) && (shiftY<0)) || (shiftY==0))
            && (((z1<=z2) && (shiftZ>0)) || ((z1>=z2) && (shiftZ<0)) || (shiftZ==0))
            && ((shiftX!=0) || (shiftY!=0) || (shiftZ!=0)))
        {
            bool check=false;
            if (y0<x0)
            {
                if (y0<z0)
                {
                    y0+=stepY;
                    y1+=shiftY;
                }
                else
                {
                    check=true;
                    z0+=stepZ;
                    z1+=shiftZ;
                }
            }
            else if (x0<z0)
            {
                x0+=stepX;
                x1+=shiftX;
            }
            else
            {
                check=true;
                z0+=stepZ;
                z1+=shiftZ;
            }

            if ((check) && (shiftZ==1) && (WORLD_MAP->cell(x1,y1,z1-1)->blocksVerticalLOS()))
                return false;

            const auto cell=WORLD_MAP->cell(x1,y1,z1);
            if (((cell->blocksLOS()) || (check)) && (shiftZ==-1) && (cell->blocksVerticalLOS()))
                return false;

            if ((x1==x2) && (y1==y2) && (z1==z2))
                return true;
        }

        return false;
    }

    void Character::spotted()
    {
        if ((mCurrentTarget!=nullptr) && (!shouldRunFromTarget()))
            FORTRESS->military()->addDefendTarget(std::dynamic_pointer_cast<Character>(shared_from_this()));

        if (mHasBeenSpotted)
            return;

        mHasBeenSpotted=true;

        std::string log_str;

        if (!hasUniqueName())
        {
            log_str+="A ";
        }

        log_str+=name()+" has been spotted.";
        std::cout << log_str << std::endl;
    }

    std::shared_ptr<Character> Character::checkEnemyLOS() const
    {
        const auto factionDef=mHistory->factionDef();
        if (factionDef==nullptr)
            return nullptr;

        const auto faction=AIDIRECTOR->faction(mHistory->factionID());
        const auto squad_pos=squadPosition();
        const auto sightRadius=raceDefinition()->SightRadius;
        const auto sightSquare=sqrtf((sightRadius*sightRadius)*formationVisionBonus());
        const auto factions=AIDIRECTOR->factions();
        std::shared_ptr<Character> character=nullptr;
        for (const auto& enemyFaction : factions)
        {
            if (faction->isHostile(enemyFaction->id()))
            {
                const auto members=enemyFaction->members();
                for (const auto& member : members)
                {
                    if (member->ID()==ID())
                        continue;

                    if (member->isOffMap())
                        continue;

                    float distance=map::vector3::distance(member->position(),position());
                    if ((distance<=sightSquare) && (distance<member->minimumVisibleDistance()))
                    {
                        if (character!=nullptr)
                        {
                            bool isFinishHim=true;
                            if ((squad_pos!=nullptr) && (squad_pos->perk()==SquadPositionPerk::FinishHim))
                            {
                                if (character->body()->containStatusEffect(HealthStatusAilment::FallenOver))
                                {
                                    if (!member->body()->containStatusEffect(HealthStatusAilment::FallenOver))
                                        continue;
                                }
                                else if (member->body()->containStatusEffect(HealthStatusAilment::FallenOver))
                                    isFinishHim=false;
                            }

                            if ((isFinishHim) && (distance>map::vector3::distance(character->position(),position())))
                                continue;
                        }

                        bool areEnemiesVisible=WORLD_MAP->cell(member->position())->areEnemiesVisible();
                        if (((faction->id()!=AIDIRECTOR->playerFactionID()) || (areEnemiesVisible))
                            && (hasLOS(position(),member->position())))
                        {
                            if (mHistory->factionID()==AIDIRECTOR->playerFactionID())
                                member->spotted();

                            if ((canReach(member->position(),false))
                                || (mBody->hasWeaponInRange(map::vector3::distance(position(),member->position()))))
                                character=member;
                        }
                    }
                }
            }
        }

        return character;
    }

    bool Character::canTarget(std::shared_ptr<Character> target) const
    {
        if (canReach(target->position(),false))
            return true;

        if (hasLOS(position(),target->position()))
            return mBody->hasWeaponInRange(map::vector3::distance(position(),target->position()));

        return false;
    }

    void Character::enterCombat(std::shared_ptr<Character> target)
    {
        if (target!=nullptr)
            std::cout << name() << " targeted " << target->name() << std::endl;

        mCurrentTarget=target;
    }

    bool Character::shouldTrackEnemy(std::shared_ptr<Character> target) const
    {
        if (mSquad==nullptr)
            return false;

        if (isRetreatingToHeal())
            return false;

        if (isRetreatingToEquip())
            return false;

        if (isRetreatingToRefillAmmo())
            return false;

        const auto squad_pos=squadPosition();
        return ((squad_pos!=nullptr) && (((squad_pos->pursueEnemies()) && (canReach(target->position(),false)))
            || ((mSquad->formation()!=nullptr) && (mSquad->isAttackTarget(target)))));
    }

    void Character::checkSurroundings(const float& dt)
    {
        const int lightRadius=castLightRadius();
        if (lightRadius>0)
        {
            mCastLightTimer-=dt;
            if (mCastLightTimer<0.0f)
            {
                mCastLightTimer=1.0f/mBody->moveSpeed();
                const bool isBlind=mBody->containStatusEffect(HealthStatusAilment::Blind);
                clearVisibleList();
                castLight(isBlind ? 2 : lightRadius);
            }
        }

        std::shared_ptr<Character> target=nullptr;
        std::shared_ptr<Character> leaderTarget=squadLeaderTarget();
        if ((leaderTarget!=nullptr) && ((mSquad==nullptr) || (!mSquad->isInMembers(leaderTarget))) && (canTarget(leaderTarget)))
            target=leaderTarget;
        else
            target=checkEnemyLOS();

        if (target!=nullptr)
        {
            mLastEnemyLocation=target->position();
            if ((mCurrentTarget!=nullptr) && (mCurrentTarget->ID()==target->ID()))
                return;

            if ((mCurrentTarget==nullptr) || (mCurrentTarget->body()== nullptr) || (mCurrentTarget->body()->isDead()))
            {
                enterCombat(target);
            }
            else
            {
                const auto squad_pos=(mCurrentTarget!=nullptr) ? mCurrentTarget->squadPosition() : nullptr;
                if ((squad_pos!=nullptr) && (squad_pos->perk()==SquadPositionPerk::Taunt))
                    return;

                enterCombat(target);
            }
        }
        else if (mCurrentTarget!=nullptr)
        {
            if ((mCurrentTarget->body()==nullptr) || (mCurrentTarget->body()->isDead()))
            {
                mTrackEnemy = false;
            }
            else if ((!shouldRunFromTarget()) && (shouldTrackEnemy(mCurrentTarget)))
            {
                mTrackEnemy=true;
                mLastEnemyLocation=mCurrentTarget->position();
            }

            if ((mSquad!=nullptr) && (mSquad->isInMembers(mCurrentTarget)))
            {
                const auto station=mSquad->station();
                if (station!=nullptr)
                {
                    if (std::dynamic_pointer_cast<TrainingStation>(station)!=nullptr)
                        return;
                }
            }

            leaveCombat();
        }
        else
        {
            if ((mSquad!=nullptr) && (mSquad->isInMembers(mCurrentTarget)))
                return;

            leaveCombat();
        }
    }

    std::vector<std::string> Character::templateMaterialIDs() const
    {
        return mHistory->templateMaterialIDs();
    }

    void Character::removingAttackTarget(std::shared_ptr<Character> target)
    {
        if ((mCurrentTarget!=nullptr) && (mCurrentTarget->ID()==target->ID()))
            return;

        if ((mSquad==nullptr) || (!mSquad->isInMembers(mCurrentTarget)))
            leaveCombat();

        mCurrentTarget=nullptr;
        mTrackEnemy=false;

        if (desiredPosition()!=mLastEnemyLocation)
            return;

        mPathfinder->clearPath();

        mMoveTime=0.0f;
    }
}