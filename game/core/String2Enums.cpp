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

    Material String2Enums::str2Material(const std::string& str)
    {
        if (str=="StandardBone")
            return Material::StandardBone;
        else if (str=="StandardShell")
            return Material::StandardShell;
        else if (str=="StandardSkin")
            return Material::StandardSkin;
        else if (str=="StandardWeakFlesh")
            return Material::StandardWeakFlesh;
        else if (str=="StandardStrongFlesh")
            return Material::StandardStrongFlesh;
        else
            return Material::Count;
    }
}