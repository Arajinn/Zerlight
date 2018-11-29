//
// Created by tatiana.polozova on 26.03.2018.
//

#ifndef ZERL_ENUMS_H
#define ZERL_ENUMS_H

namespace game
{
    enum class CharacterAttributeType
    {
        Fitness = 0,
        Nimbleness = 1,
        Curiosity = 2,
        Focus = 3,
        Charm = 4,
        Undef=5,
        Size
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
        BodyFluid = 13
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

    enum class Material
    {
        Air = 0,
        Grass = 1,
        Dirt = 2,
        Clay = 3,
        Granite = 4,
        Basalt = 5,
        Marble = 6,
        Sandstone = 7,
        Bauxite = 8,
        Serpentine = 9,
        LapisLazuli = 10,
        Obsidian = 11,
        Brick = 12,
        Water = 13,
        Lava = 14,
        Oak = 15,
        AppleWood = 16,
        Birch = 17,
        Coal = 18,
        Copper = 19,
        Malachite = 20,
        Tin = 21,
        Bronze = 22,
        Iron = 23,
        Steel = 24,
        Lead = 25,
        Silver = 26,
        Gold = 27,
        RoseGold = 28,
        Platinum = 29,
        GreenGem = 30,
        BlueGem = 31,
        Strawberry = 32,
        Apple = 33,
        Wheat = 34,
        Cotton = 35,
        Wool = 36,
        StandardBone = 37,
        StandardShell = 38,
        ThinChitin = 39,
        ThickChitin = 40,
        StandardSkin = 41,
        YakHide = 42,
        BearHide = 43,
        StandardWeakFlesh = 44,
        StandardStrongFlesh = 45,
        Milk = 46,
        Egg = 47,
        OgreHide = 48,
        ToughOgreHide = 49,
        ThinMantChitin = 50,
        ThickMantChitin = 51,
        AlpacaHide = 52,
        EmuHide = 53,
        HoneyBadgerHide = 54,
        MonitorLizardHide = 55,
        YakFlesh = 56,
        AlpacaFlesh = 57,
        EmuFlesh = 58,
        HoneyBadgerFlesh = 59,
        MonitorLizardFlesh = 60,
        BearFlesh = 61,
        OgreFlesh = 62,
        TwoHeadedOgreFlesh = 63,
        YakBone = 64,
        AlpacaBone = 65,
        EmuBone = 66,
        HoneyBadgerBone = 67,
        MonitorLizardBone = 68,
        BearBone = 69,
        OgreBone = 70,
        TwoHeadedOgreBone = 71,
        YakSkull = 72,
        AlpacaSkull = 73,
        EmuSkull = 74,
        HoneyBadgerSkull = 75,
        MonitorLizardSkull = 76,
        BearSkull = 77,
        OgreSkull = 78,
        TwoHeadedOgreSkull = 79,
        Grape = 80,
        Orange = 81,
        OrangeWood = 82,
        Mushroom = 83,
        Silica = 84,
        Ceramic = 85,
        Glass = 86,
        Count = 87
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
        Bleeding = 4,
        StruckArtery = 8,
        InternalBleeding = 16,
        Artificial = 32,
    };
}

#endif //ZERL_ENUMS_H
