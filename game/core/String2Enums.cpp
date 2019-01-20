//
// Created by tatiana on 07.12.18.
//

#include "String2Enums.h"

namespace game
{
    MaterialType String2Enums::str2MaterialType(const std::string& str)
    {
        if (str=="Air")
            return MaterialType::Air;
        else if (str=="Soil")
            return MaterialType::Soil;
        else if (str=="Stone")
            return MaterialType::Stone;
        else if (str=="Liquid")
            return MaterialType::Liquid;
        else if (str=="Wood")
            return MaterialType::Wood;
        else if (str=="Coal")
            return MaterialType::Coal;
        else if (str=="Metal")
            return MaterialType::Metal;
        else if (str=="Gem")
            return MaterialType::Gem;
        else if (str=="Plant")
            return MaterialType::Plant;
        else if (str=="Cloth")
            return MaterialType::Cloth;
        else if (str=="Bone")
            return MaterialType::Bone;
        else if (str=="Hide")
            return MaterialType::Hide;
        else if (str=="Flesh")
            return MaterialType::Flesh;
        else if (str=="BodyFluid")
            return MaterialType::BodyFluid;
        else
            return MaterialType::Undef;
    }

    ItemEffectType String2Enums::str2ItemEffectType(const std::string& str)
    {
        if (str=="Food")
            return ItemEffectType::Food;
        else if (str=="Drink")
            return ItemEffectType::Drink;
        else if (str=="Heal")
            return ItemEffectType::Heal;
        else if (str=="Storage")
            return ItemEffectType::Storage;
        else if (str=="Rots")
            return ItemEffectType::Rots;
        else if (str=="Sleep")
            return ItemEffectType::Sleep;
        else if (str=="Sit")
            return ItemEffectType::Sit;
        else if (str=="Surface")
            return ItemEffectType::Surface;
        else if (str=="Art")
            return ItemEffectType::Art;
        else if (str=="Scary")
            return ItemEffectType::Scary;
        else if (str=="LightSource")
            return ItemEffectType::LightSource;
        else
            return ItemEffectType::None;
    }

    BodyPartProperty String2Enums::str2BodyPartProperty(const std::string& str)
    {
        if (str=="Splinters")
            return BodyPartProperty::Splinters;
        else if (str=="HasBlood")
            return BodyPartProperty::HasBlood;
        else if (str=="HasArtery")
            return BodyPartProperty::HasArtery;
        else if (str=="Nauseates")
            return BodyPartProperty::Nauseates;
        else
            return BodyPartProperty::None;
    }

    BodyFunction String2Enums::str2BodyFunction(const std::string& str)
    {
        if (str=="Thought")
            return BodyFunction::Thought;
        else if (str=="Circulation")
            return BodyFunction::Circulation;
        else if (str=="Sight")
            return BodyFunction::Sight;
        else if (str=="Breathe")
            return BodyFunction::Breathe;
        else if (str=="Throat")
            return BodyFunction::Throat;
        else if (str=="InternalOrgan")
            return BodyFunction::InternalOrgan;
        else if (str=="Structure")
            return BodyFunction::Structure;
        else if (str=="Motor")
            return BodyFunction::Motor;
        else if (str=="Grip")
            return BodyFunction::Grip;
        else if (str=="Stand")
            return BodyFunction::Stand;
        else if (str=="Wing")
            return BodyFunction::Wing;
        else
            return BodyFunction::None;
    }

    EquipmentType String2Enums::str2EquipmentType(const std::string& str)
    {
        if (str=="Head")
            return EquipmentType::Head;
        else if (str=="Body")
            return EquipmentType::Body;
        else if (str=="Arm")
            return EquipmentType::Arm;
        else if (str=="Leg")
            return EquipmentType::Leg;
        else if (str=="Glove")
            return EquipmentType::Glove;
        else if (str=="Foot")
            return EquipmentType::Foot;
        else if (str=="LeftHand")
            return EquipmentType::LeftHand;
        else if (str=="RightHand")
            return EquipmentType::RightHand;
        else if (str=="Back")
            return EquipmentType::Back;
        else if (str=="Belt")
            return EquipmentType::Belt;
        else
            return EquipmentType::None;
    }

    AttackType String2Enums::str2AttackType(const std::string& str)
    {
        if (str=="Swing")
            return AttackType::Swing;
        else if (str=="Thrust")
            return AttackType::Thrust;
        else if (str=="HandToHand")
            return AttackType::HandToHand;
        else if (str=="Projectile")
            return AttackType::Projectile;
        else if (str=="Temperature")
            return AttackType::Temperature;
        else if (str=="Gas")
            return AttackType::Gas;
        else if (str=="Mental")
            return AttackType::Mental;
        else
            return AttackType::Undef;
    }

    DamageType String2Enums::str2DamageType(const std::string& str)
    {
        if (str=="Slash")
            return DamageType::Slash;
        else if (str=="Pierce")
            return DamageType::Pierce;
        else if (str=="Blunt")
            return DamageType::Blunt;
        else if (str=="Heat")
            return DamageType::Heat;
        else if (str=="Cold")
            return DamageType::Cold;
        else if (str=="Poison")
            return DamageType::Poison;
        else
            return DamageType::Undef;
    }

    GenderType String2Enums::str2GenderType(const std::string& str)
    {
        if (str=="Male")
            return GenderType::Male;
        else if (str=="Female")
            return GenderType::Female;
        else if (str=="Neuter")
            return GenderType::Neuter;
        else
            return GenderType::Undef;
    }

    CharacterAttributeType String2Enums::str2CharacterAttributeType(const std::string& str)
    {
        if (str=="Fitness")
            return CharacterAttributeType::Fitness;
        else if (str=="Nimbleness")
            return CharacterAttributeType::Nimbleness;
        else if (str=="Curiosity")
            return CharacterAttributeType::Curiosity;
        else if (str=="Focus")
            return CharacterAttributeType::Focus;
        else if (str=="Charm")
            return CharacterAttributeType::Charm;
        else
            return CharacterAttributeType::Undef;
    }

    FactionType String2Enums::str2FactionType(const std::string& str)
    {
        if (str=="PlayerCiv")
            return FactionType::PlayerCiv;
        else if (str=="FriendlyCiv")
            return FactionType::FriendlyCiv;
        else if (str=="Neutral")
            return FactionType::Neutral;
        else if (str=="Wild")
            return FactionType::Wild;
        else if (str=="Subteranean")
            return FactionType::Subteranean;
        else if (str=="EnemyCiv")
            return FactionType::EnemyCiv;
        else if (str=="EnemySmall")
            return FactionType::EnemySmall;
        else if (str=="Mant")
            return FactionType::Mant;
        else if (str=="Beetle")
            return FactionType::Beetle;
        else if (str=="Undead")
            return FactionType::Undead;
        else
            return FactionType::Undef;
    }

    SquadType String2Enums::str2SquadType(const std::string& str)
    {
        if (str=="Attack")
            return SquadType::Attack;
        else if (str=="MantScout")
            return SquadType::MantScout;
        else if (str=="Merchant")
            return SquadType::Merchant;
        else if (str=="Ambassador")
            return SquadType::Ambassador;
        else if (str=="GoblinRaider")
            return SquadType::GoblinRaider;
        else if (str=="GoblinTunneler")
            return SquadType::GoblinTunneler;
        else if (str=="Automaton")
            return SquadType::Automaton;
        else if (str=="BeetleCocoon")
            return SquadType::BeetleCocoon;
        else if (str=="Golem")
            return SquadType::Golem;
        else
            return SquadType::Undef;
    }
}