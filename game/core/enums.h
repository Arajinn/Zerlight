//
// Created by tatiana on 26.03.2018.
//

#ifndef ZERL_ENUMS_H
#define ZERL_ENUMS_H

namespace game
{
    enum class view_orientation
    {
        front_left,
        front_right,
        back_right,
        back_left
    };

    enum class CharacterAttributeType
    {
        Fitness = 0,
        Nimbleness = 1,
        Curiosity = 2,
        Focus = 3,
        Charm = 4,
        Undef=5
    };

    enum class HealthStatusAilment
    {
        Unconcious,
        Dazed,
        Faint,
        Winded,
        Dizzy,
        FallenOver,
        Grounded,
        Blind,
        ZombieVirus,
        Size
    };

    enum class SquadPositionPerk
    {
        None,
        Highlander,
        Frenzy,
        WayOfTheGnome,
        Scout,
        Guard,
        Marksgnome,
        Sharpshooter,
        FinishHim,
        Cripple,
        Disarm,
        Blind,
        Taunt,
        Commander,
        SurvivalInstinct,
        Sneaky,
        Conservationist,
        Swashbuckler,
        Size
    };

    enum class ConstructionProperty
    {
        None = 0,
        BlocksMovement = 1,
        BlocksLOS = 2,
        BlocksVerticalLOS = 3,
        Size
    };

    enum class ConstructOrientation
    {
        FrontLeft = 0,
        FrontRight = 1,
        BackRight = 2,
        BackLeft = 3,
        Size = 4
    };

    enum class ItemEffectType
    {
        Food = 0,
        Drink = 1,
        Heal = 2,
        Storage = 3,
        Rots = 4,
        Sleep = 5,
        Sit = 6,
        Surface = 7,
        Art = 8,
        Scary = 9,
        LightSource = 10,
        None
    };

    enum class MaterialType
    {
        Air = 0,
        Soil = 1,
        Stone = 2,
        Liquid = 3,
        Wood = 4,
        Coal = 5,
        Metal = 6,
        Gem = 7,
        Plant = 8,
        Cloth = 9,
        Bone = 10,
        Hide = 11,
        Flesh = 12,
        BodyFluid = 13,
        Undef
    };

    enum class ItemQuality
    {
        Poor = 0,
        Average = 1,
        Fine = 2,
        Superior = 3,
        Masterful = 4,
        Legendary = 5,
        Any = 6
    };

    enum class EquipmentType
    {
        Head = 0,
        Body = 1,
        Arm = 2,
        Leg = 3,
        Glove = 4,
        Foot = 5,
        LeftHand = 6,
        RightHand = 7,
        Back = 8,
        Belt = 9,
        None = 10,
        Count = 11
    };

    enum class BodyPartProperty
    {
        None = 0,
        Splinters = 1,
        HasBlood = 2,
        HasArtery = 4,
        Nauseates = 8
    };

    enum class BodyFunction
    {
        None = 0,
        Thought = 1,
        Circulation = 2,
        Sight = 3,
        Breathe = 4,
        Throat = 5,
        InternalOrgan = 6,
        Structure = 7,
        Motor = 8,
        Grip = 9,
        Stand = 10,
        Wing = 11,
        Count = 12
    };

    enum class BodyPartStatus
    {
        Good = 0,
        Disabled = 1,
        Missing = 2,
        ExposesContainedParts = 4,
        Bleeding = 8,
        StruckArtery = 16,
        Poisoned = 32,
        Severed = 64,
    };

    enum class BodySectionStatus
    {
        Good = 0,
        Destroyed = 1,
        Missing = 2,
        Bleeding = 3,
        StruckArtery = 4,
        InternalBleeding = 5,
        Artificial = 6,
        Undef
    };

    enum class BodySymmetricalType
    {
        Left,
        Right,
        None
    };

    enum class GenderType
    {
        Neuter = 0,
        Male = 1,
        Female = 2,
        Undef
    };

    enum class FactionType
    {
        PlayerCiv = 0,
        FriendlyCiv = 1,
        Neutral = 2,
        Wild = 3,
        Subteranean = 4,
        EnemyCiv = 5,
        EnemySmall = 6,
        Mant = 7,
        Beetle = 8,
        Undead = 9,
        Undef
    };

    enum class AttackType
    {
        Swing = 0,
        Thrust = 1,
        HandToHand = 2,
        Projectile = 3,
        Temperature = 4,
        Gas = 5,
        Mental = 6,
        Undef
    };

    enum class FormationPerk
    {
        None,
        KeepYourEyesOpen,
        ShieldWall,
        BestDefense,
        FiringSquad,
        FireAtWill,
        SuppressiveFire,
    };

    enum class DamageType
    {
        Slash = 0,
        Pierce = 1,
        Blunt = 2,
        Heat = 3,
        Cold = 4,
        Poison = 5,
        Undef
    };

    enum class WeaponStatusEffectType
    {
        None = 0,
        ZombieVirus = 1
    };

    enum class SquadType
    {
        Attack = 0,
        MantScout = 1,
        Merchant = 2,
        Ambassador = 3,
        GoblinRaider = 4,
        GoblinTunneler = 5,
        Automaton = 6,
        BeetleCocoon = 7,
        Golem = 8,
        Undef
    };

    enum class EnvoyState
    {
        Departing,
        OnLocation,
        Returning,
        Finished
    };
}

#endif //ZERL_ENUMS_H
