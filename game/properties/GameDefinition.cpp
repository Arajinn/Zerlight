//
// Created by tatiana on 27.03.2018.
//

#include "GameDefinition.h"
#include "TerrainSettings.h"
#include "RaceDefinition.h"
#include "ConstructionDef.h"
#include "ItemDefinition.h"
#include "GameSettings.h"
#include "BodyPartDef.h"
#include "BodySectionDef.h"
#include "BodyDef.h"
#include "MaterialDef.h"
#include "SpriteDef.h"
#include "FactionDef.h"
#include "UniformSettings.h"
#include "ItemSettings.h"
#include "AmmoDef.h"
#include "GenderDef.h"
#include "CharacterSettings.h"
#include "WeaponDef.h"
#include "AttackDef.h"
#include "TargetedAttackDef.h"
#include "DefendDef.h"
#include "NaturalWeaponDef.h"
#include "SquadDef.h"
#include "RaceClassDef.h"
#include "DamageDef.h"
#include "game/core/String2Enums.h"
#include "game/core/defines.h"
#include "game/core/Item.h"
#include "game/properties/MaterialDef.h"
#include "guichan/gui/ZColor.h"
#include "guichan/gui/ZImage.h"
#include "guichan/sdl/OpenGLImage.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <algorithm>
#include <regex>

namespace properties {
    GameDefinition::GameDefinition()
    {
        mTerrainSettings=std::make_shared<TerrainSettings>();
        mTerrainSettings->AirMaterialID="Air";
        mTerrainSettings->DirtMaterialID="Dirt";
        mTerrainSettings->ClayMaterialID="Clay";
        mTerrainSettings->WaterMaterialID="Water";
        mTerrainSettings->LavaMaterialID="Lava";
        mTerrainSettings->GrownOnMaterialID="Grass";
        mTerrainSettings->parseTerrainSettingsFile();

        parseMaterialDefs();

        parseSpriteDefsFile();

        parseRaceDefs();

        std::shared_ptr<ConstructionDef> soilStairs=std::make_shared<ConstructionDef>();
        soilStairs->ID="SoilStairs";
        soilStairs->Name="stairs";
        soilStairs->Prefix="soil";
        mConstructionDefs.push_back(soilStairs);

        std::shared_ptr<ConstructionDef> rawStoneRamp=std::make_shared<ConstructionDef>();
        rawStoneRamp->ID="RawStoneRamp";
        rawStoneRamp->Name="ramp";
        rawStoneRamp->Prefix="stone";
        mConstructionDefs.push_back(rawStoneRamp);

        parseItemDefs();

        mGameSettings=std::make_shared<GameSettings>();
        mGameSettings->NutritionWeight=50.0f;
        mGameSettings->DepthDistanceBias=1.0f;

        parseBodyPartDefs();
        //this->initBodyPartDefs();
        parseBodyDefs();
        //this->initBodyDefs();

        parseFactionDefs();

        mItemSettings=std::make_shared<ItemSettings>();
        mItemSettings->CorpseItemID="Corpse";
        mItemSettings->LimbItemID="BodyPart";

        mUniformSettings=std::make_shared<UniformSettings>();

        mCharacterSettings=std::make_shared<CharacterSettings>();
    };

    GameDefinition::~GameDefinition()
    {

    }

    const std::shared_ptr<TerrainSettings>& GameDefinition::terrainSettings() const
    {
        return mTerrainSettings;
    }

    int GameDefinition::indexOfMaterial(std::string id) const
    {
        auto iter=std::find_if(mMaterialDefs.begin(),mMaterialDefs.end(),[&id](std::shared_ptr<MaterialDef> const& value)
        {
           return (id==value->ID);
        });

        if (iter==mMaterialDefs.end())
            return -1;
        else
            return (int)std::distance(mMaterialDefs.begin(),iter);
    }

    std::shared_ptr<const RaceDefinition> GameDefinition::raceDefinition(const std::string& id) const
    {
        auto iter=std::find_if(mRaceDefinitions.begin(),mRaceDefinitions.end(),[&id](std::shared_ptr<RaceDefinition> const& value)
        {
            return (id==value->ID);
        });

        if (iter!=mRaceDefinitions.end())
            return (*iter);
        else
            return nullptr;
    }

    std::shared_ptr<const ConstructionDef> GameDefinition::constructionDefinition(const std::string& id) const
    {
        auto iter=std::find_if(mConstructionDefs.begin(),mConstructionDefs.end(),[&id](std::shared_ptr<ConstructionDef> const& value)
        {
            return (id==value->ID);
        });

        if (iter!=mConstructionDefs.end())
            return (*iter);
        else
            return nullptr;
    }

    std::shared_ptr<const ItemDefinition> GameDefinition::itemDefinition(const std::string& id) const
    {
        auto iter=std::find_if(mItemDefinitions.begin(),mItemDefinitions.end(),[&id](std::shared_ptr<ItemDefinition> const& value)
        {
            return (id==value->ID);
        });

        if (iter!=mItemDefinitions.end())
            return (*iter);
        else
            return nullptr;
    }

    std::shared_ptr<const GameSettings> GameDefinition::gameSettings() const
    {
        return mGameSettings;
    }

    std::shared_ptr<const BodyPartDef> GameDefinition::bodyPartDefinition(const std::string& id) const
    {
        auto iter=std::find_if(mBodyPartDefs.begin(),mBodyPartDefs.end(),[&id](std::shared_ptr<BodyPartDef> const& value)
        {
            return (id==value->ID);
        });

        if (iter!=mBodyPartDefs.end())
            return (*iter);
        else
            return nullptr;
    }

    std::shared_ptr<const BodyDef> GameDefinition::bodyDefinition(const std::string& id) const
    {
        auto iter=std::find_if(mBodyDefs.begin(),mBodyDefs.end(),[&id](std::shared_ptr<BodyDef> const& value)
        {
            return (id==value->ID);
        });

        if (iter!=mBodyDefs.end())
            return (*iter);
        else
            return nullptr;
    }

    std::shared_ptr<const MaterialDef> GameDefinition::materialDefinition(const game::MaterialID_t& id) const
    {
        auto iter=std::find_if(mMaterialDefs.begin(),mMaterialDefs.end(),[&id](std::shared_ptr<MaterialDef> const& value)
        {
            return (id==value->ID);
        });

        if (iter!=mMaterialDefs.end())
            return (*iter);
        else
            return nullptr;
    }

    std::shared_ptr<const MaterialDef> GameDefinition::materialDefinition(const int& index) const
    {
        return mMaterialDefs.at(index);
    }

    void GameDefinition::parseMaterialDefs()
    {
        std::string path="../settings/objects/material.xml";

        boost::property_tree::ptree pt;
        boost::property_tree::read_xml(path, pt);

        for (auto const& tree_item : pt.get_child("Materials"))
        {
            if (tree_item.first=="Item")
            {
                std::shared_ptr<MaterialDef> newMaterialDef=std::make_shared<MaterialDef>();
                const auto materialID=tree_item.second.get("ID","");
                newMaterialDef->ID=materialID;
                newMaterialDef->Name=tree_item.second.get("Name","");
                const auto typeStr=tree_item.second.get("Type","");
                const auto typeEnum=game::String2Enums::str2MaterialType(typeStr);
                if (typeEnum==game::MaterialType::Stone)
                    mTerrainSettings->addStoneID(materialID);

                newMaterialDef->Type=typeEnum;
                newMaterialDef->Strength=tree_item.second.get("Strength",0.0f);
                newMaterialDef->Value=tree_item.second.get("Value",0.0f);
                newMaterialDef->Sustains=tree_item.second.get("Sustains",1.0f);
                newMaterialDef->PierceModifier=tree_item.second.get("PierceModifier",1.0f);
                newMaterialDef->SlashModifier=tree_item.second.get("SlashModifier",1.0f);
                newMaterialDef->BluntModifier=tree_item.second.get("BluntModifier",1.0f);
                std::string colorStr=tree_item.second.get("Color","");
                newMaterialDef->Color=gui::ZColor(colorStr);

                for (const auto& child : tree_item.second)
                {
                    if (child.first == "Slash")
                    {
                        newMaterialDef->DamageProperties[(int)game::DamageType::Slash]->Blocks=child.second.get("Blocks",1000.0f);
                        //newMaterialDef->DamageProperties[(int)game::DamageType::Slash]->Break=child.second.get("Break",100.0f);
                        newMaterialDef->DamageProperties[(int)game::DamageType::Slash]->PercentTransfered=child.second.get("PercentTransfered",0.0f);
                        newMaterialDef->DamageProperties[(int)game::DamageType::Slash]->PercentReceived=child.second.get("PercentReceived",1.0f);
                    }
                    else if (child.first == "Blunt")
                    {
                        newMaterialDef->DamageProperties[(int)game::DamageType::Blunt]->Blocks=child.second.get("Blocks",1000.0f);
                        //newMaterialDef->DamageProperties[(int)game::DamageType::Blunt]->Break=child.second.get("Break",100.0f);
                        newMaterialDef->DamageProperties[(int)game::DamageType::Blunt]->PercentTransfered=child.second.get("PercentTransfered",0.0f);
                        newMaterialDef->DamageProperties[(int)game::DamageType::Blunt]->PercentReceived=child.second.get("PercentReceived",1.0f);
                    }
                    else if (child.first == "Heat")
                    {
                        newMaterialDef->DamageProperties[(int)game::DamageType::Heat]->Blocks=child.second.get("Blocks",1000.0f);
                        //newMaterialDef->DamageProperties[(int)game::DamageType::Heat]->Break=child.second.get("Break",100.0f);
                        newMaterialDef->DamageProperties[(int)game::DamageType::Heat]->PercentTransfered=child.second.get("PercentTransfered",0.0f);
                        newMaterialDef->DamageProperties[(int)game::DamageType::Heat]->PercentReceived=child.second.get("PercentReceived",1.0f);
                    }
                    else if (child.first == "Cold")
                    {
                        newMaterialDef->DamageProperties[(int)game::DamageType::Cold]->Blocks=child.second.get("Blocks",1000.0f);
                        //newMaterialDef->DamageProperties[(int)game::DamageType::Cold]->Break=child.second.get("Break",100.0f);
                        newMaterialDef->DamageProperties[(int)game::DamageType::Cold]->PercentTransfered=child.second.get("PercentTransfered",0.0f);
                        newMaterialDef->DamageProperties[(int)game::DamageType::Cold]->PercentReceived=child.second.get("PercentReceived",1.0f);
                    }
                    else if (child.first == "Pierce")
                    {
                        newMaterialDef->DamageProperties[(int)game::DamageType::Pierce]->Blocks=child.second.get("Blocks",1000.0f);
                        newMaterialDef->DamageProperties[(int)game::DamageType::Pierce]->Break=child.second.get("Break",100.0f);
                        newMaterialDef->DamageProperties[(int)game::DamageType::Pierce]->PercentTransfered=child.second.get("PercentTransfered",0.0f);
                        newMaterialDef->DamageProperties[(int)game::DamageType::Pierce]->PercentReceived=child.second.get("PercentReceived",1.0f);
                    }
                    else if (child.first == "Poison")
                    {
                        newMaterialDef->DamageProperties[(int)game::DamageType::Poison]->Blocks=child.second.get("Blocks",1000.0f);
                        //newMaterialDef->DamageProperties[(int)game::DamageType::Poison]->Break=child.second.get("Break",100.0f);
                        newMaterialDef->DamageProperties[(int)game::DamageType::Poison]->PercentTransfered=child.second.get("PercentTransfered",0.0f);
                        newMaterialDef->DamageProperties[(int)game::DamageType::Poison]->PercentReceived=child.second.get("PercentReceived",1.0f);
                    }
                }

                for (size_t i=0,isize=newMaterialDef->DamageProperties.size();i<isize;i++)
                    newMaterialDef->DamageProperties[i]->Break=newMaterialDef->Sustains;

                mMaterialDefs.push_back(newMaterialDef);

                if (newMaterialDef->ID==mTerrainSettings->AirMaterialID)
                    mAirMaterialIndex=mMaterialDefs.size()-1;
            }
        }
    }

    int GameDefinition::airMaterialIndex() const
    {
        return mAirMaterialIndex;
    }

    void GameDefinition::parseSpriteDefsFile()
    {
        std::string path="../settings/objects/sprites.xml";
        std::string image_path="../images/textures/";

        boost::property_tree::ptree pt;
        boost::property_tree::read_xml(path, pt);

        for (const auto& tree_item : pt.get_child("Sprites"))
        {
            if (tree_item.first=="Item")
            {
                std::shared_ptr<SpriteDef> newSpriteDef=std::make_shared<SpriteDef>();
                newSpriteDef->SpriteID=tree_item.second.get("SpriteID","");
                newSpriteDef->SpritePath=tree_item.second.get("Source","");
                std::string filePath=image_path+newSpriteDef->SpritePath;
                newSpriteDef->Image=gui::ZImage::load(filePath);

                mSpriteDefs.push_back(newSpriteDef);
            }
        }

        std::sort(mSpriteDefs.begin(), mSpriteDefs.end(),
             [](const std::shared_ptr<SpriteDef> & left, const std::shared_ptr<SpriteDef> & right) -> bool
             {
                 return left->SpriteID < right->SpriteID;
             });
    }

    std::shared_ptr<const SpriteDef> GameDefinition::spriteDefinition(const std::string& spriteID) const
    {
//        auto iter=std::find_if(mSpriteDefs.begin(),mSpriteDefs.end(),[&spriteID](std::shared_ptr<SpriteDef> const& item)
//        {
//            return (item->SpriteID==spriteID);
//        });

        auto iter = std::lower_bound(mSpriteDefs.begin(), mSpriteDefs.end(),spriteID,
                [](const std::shared_ptr<SpriteDef>& left, const std::string& right)
                {
                    return left->SpriteID  < right;
                });

        if (iter!=mSpriteDefs.end())
            return (*iter);
        else
            return nullptr;
    }

    void GameDefinition::parseItemDefs()
    {
        std::string path="../settings/objects/items.xml";

        boost::property_tree::ptree pt;
        boost::property_tree::read_xml(path, pt);

        for (const auto& tree_item : pt.get_child("Items"))
        {
            if (tree_item.first=="Item")
            {
                std::shared_ptr<ItemDefinition> newItemDefinition=std::make_shared<ItemDefinition>();
                newItemDefinition->ID=tree_item.second.get("ID","");
                newItemDefinition->Name=tree_item.second.get("Name","");
                newItemDefinition->Prefix=tree_item.second.get("Prefix","");
                newItemDefinition->Suffix=tree_item.second.get("Suffix","");

                newItemDefinition->Thickness=tree_item.second.get("Thickness",1.0f);

                newItemDefinition->CombatRatingModifier=tree_item.second.get("CombatRatingModifier",0.0f);
                newItemDefinition->WeaponSize=tree_item.second.get("WeaponSize",1.0f);
                newItemDefinition->TwoHanded=tree_item.second.get("TwoHanded",false);

                const std::string equipTypeStr=tree_item.second.get("EquipSlot","");
                newItemDefinition->EquipSlot=game::String2Enums::str2EquipmentType(equipTypeStr);
                newItemDefinition->EquippedJobPenalty=tree_item.second.get("EquippedJobPenalty",0.0f);
                newItemDefinition->EquippedMovePenalty=tree_item.second.get("EquippedMovePenalty",0.0f);

                for (const auto& child : tree_item.second)
                {
                    if (child.first=="Effect")
                    {
                        std::string typeStr=child.second.get("Effect","");
                        game::ItemEffectType type=game::String2Enums::str2ItemEffectType(typeStr);
                        float effectValue=child.second.get("Amount",0.0f);
                        newItemDefinition->addEffect(type,effectValue);
                    }
                    else if (child.first=="SpriteID")
                    {
                        ItemDefinition::SpriteID spriteInfo;
                        spriteInfo.spriteID=child.second.get("SpriteID","");
                        int materialId=child.second.get("MaterialIndex",0);
                        spriteInfo.useMaterial=(materialId!=-1);

                        for (const auto& sprite_child : child.second)
                        {
                            if (sprite_child.first=="SpriteIDByMaterialID")
                            {
                                std::string key=sprite_child.second.get("Key","");
                                std::string value=sprite_child.second.get("Value","");
                                spriteInfo.spriteIDByMaterialID.push_back({key,value});
                            }
                        }

                        newItemDefinition->Sprites.push_back(spriteInfo);
                    }
                    else if (child.first=="WeaponDef")
                    {
                        std::shared_ptr<WeaponDef> weaponDef=std::make_shared<WeaponDef>();
                        weaponDef->Skill=child.second.get("Skill","");
                        weaponDef->AmmoItemID=child.second.get("AmmoItemID","");
                        weaponDef->Point=child.second.get("Point",0.0f);
                        weaponDef->Edge=child.second.get("Edge",0.0f);
                        weaponDef->BluntModifier=child.second.get("BluntModifier",1.0f);
                        weaponDef->ProjectileModifier=child.second.get("ProjectileModifier",1.0f);
                        weaponDef->VelocityModifier=child.second.get("VelocityModifier",1.0f);
                        weaponDef->KnockbackModifier=child.second.get("KnockbackModifier",1.0f);

                        for (const auto& move_child : child.second)
                        {
                            if (move_child.first=="AttackMove")
                            {
                                std::shared_ptr<AttackDef> attackDef=std::make_shared<AttackDef>();

                                attackDef->Verb=move_child.second.get("Verb","");
                                const std::string attackTypeStr=move_child.second.get("AttackType","");
                                attackDef->AttackType=game::String2Enums::str2AttackType(attackTypeStr);
                                attackDef->AttackTime=move_child.second.get("AttackTime",3.0f);
                                attackDef->AttackRange=move_child.second.get("AttackRange",1.0f);
                                attackDef->RequiresAmmo=move_child.second.get("RequiresAmmo",false);
                                attackDef->Weight=move_child.second.get("Weight",1.0f);
                                attackDef->MinimumSkillLevel=move_child.second.get("MinimumSkillLevel",1);
                                attackDef->DamageScale=move_child.second.get("DamageScale",1.0f);

                                std::string damageStr=move_child.second.get("Damage","");
                                if (!damageStr.empty())
                                {
                                    std::vector<std::string> damageSplit = split(damageStr, ",");
                                    for (const auto& damage : damageSplit)
                                        attackDef->DamageTypes.push_back(game::String2Enums::str2DamageType(damage));
                                }

                                weaponDef->AttackMoves.push_back(attackDef);
                            }
                            else if (move_child.first=="DefendMove")
                            {
                                std::shared_ptr<DefendDef> defendDef=std::make_shared<DefendDef>();
                                defendDef->Verb=move_child.second.get("Verb","");
                                defendDef->DefendTime=move_child.second.get("DefendTime",1.0f);
                                defendDef->AttackDelay=move_child.second.get("AttackDelay",1.0f);

                                std::string defendTypeStr=move_child.second.get("DefendType","");
                                if (!defendTypeStr.empty())
                                {
                                    std::vector<std::string> defendTypeSplit = split(defendTypeStr, ",");
                                    for (const auto& defend : defendTypeSplit)
                                        defendDef->DefendTypes.push_back(game::String2Enums::str2AttackType(defend));
                                }

                                weaponDef->DefendMoves.push_back(defendDef);
                            }
                            else if (move_child.first=="TargetedAttackMove")
                            {
                                std::shared_ptr<TargetedAttackDef> targetedAttackDef=std::make_shared<TargetedAttackDef>();

                                std::string targetedMaterialStr=move_child.second.get("TargetedMaterial","");
                                if (!targetedMaterialStr.empty())
                                {
                                    std::vector<std::string> targetedMaterialSplit = split(targetedMaterialStr, ",");
                                    for (const auto& material : targetedMaterialSplit)
                                        targetedAttackDef->TargetedMaterials.push_back(game::String2Enums::str2MaterialType(material));
                                }

                                for (const auto& attack_child : move_child.second)
                                {
                                    if (attack_child.first=="AttackDef")
                                    {
                                        std::shared_ptr<AttackDef> attackDef=std::make_shared<AttackDef>();

                                        attackDef->Verb=attack_child.second.get("Verb","");
                                        const std::string attackTypeStr=attack_child.second.get("AttackType","");
                                        attackDef->AttackType=game::String2Enums::str2AttackType(attackTypeStr);
                                        attackDef->AttackTime=attack_child.second.get("AttackTime",3.0f);
                                        attackDef->AttackRange=attack_child.second.get("AttackRange",1.0f);
                                        attackDef->RequiresAmmo=attack_child.second.get("RequiresAmmo",false);
                                        attackDef->Weight=attack_child.second.get("Weight",1.0f);
                                        attackDef->MinimumSkillLevel=attack_child.second.get("MinimumSkillLevel",1);
                                        attackDef->DamageScale=attack_child.second.get("DamageScale",1.0f);

                                        std::string damageStr=attack_child.second.get("Damage","");
                                        if (!damageStr.empty())
                                        {
                                            std::vector<std::string> damageSplit = split(damageStr, ",");
                                            for (const auto& damage : damageSplit)
                                                attackDef->DamageTypes.push_back(game::String2Enums::str2DamageType(damage));
                                        }

                                        targetedAttackDef->TargetedAttackDef=attackDef;
                                    }
                                }

                                weaponDef->TargetedAttackMoves.push_back(targetedAttackDef);
                            }
                        }

                        newItemDefinition->ItemWeaponDef=weaponDef;
                    }
                }

                mItemDefinitions.push_back(newItemDefinition);
            }
        }
    }

    std::vector<std::string> GameDefinition::split(std::string data, std::string delimiter)
    {
        std::vector<std::string> result;
        std::string reg_str="[^"+delimiter+"]+";
        std::regex reg(reg_str);
        std::sregex_token_iterator begin(data.begin(), data.end(), reg), end;
        std::copy(begin, end, std::back_inserter(result));
        return result;
    }

    void GameDefinition::parseBodyPartDefs()
    {
        std::string path="../settings/objects/bodypart.xml";

        boost::property_tree::ptree pt;
        boost::property_tree::read_xml(path, pt);

        for (auto const& tree_item : pt.get_child("BodyParts"))
        {
            if (tree_item.first=="Item")
            {
                std::shared_ptr<BodyPartDef> newBodyPartDef=std::make_shared<BodyPartDef>();

                newBodyPartDef->ID=tree_item.second.get("ID","");
                newBodyPartDef->Name=tree_item.second.get("Name","");
                newBodyPartDef->MaterialID=tree_item.second.get("MaterialID","");

                std::string bodyProperties=tree_item.second.get("BodyProperties","");
                if (!bodyProperties.empty())
                {
                    std::vector<std::string> bodyPropertiesSplit = split(bodyProperties, ",");
                    for (const auto& bodyProperty : bodyPropertiesSplit)
                        newBodyPartDef->BodyProperties.push_back(game::String2Enums::str2BodyPartProperty(bodyProperty));
                }

                std::string bodyFunction=tree_item.second.get("BodyFunction","None");
                newBodyPartDef->BodyFunction = game::String2Enums::str2BodyFunction(bodyFunction);

                newBodyPartDef->Symmetrical = tree_item.second.get("Symmetrical",false);
                newBodyPartDef->ToHitWeight = tree_item.second.get("ToHitWeight",1.0f);
                newBodyPartDef->Thickness = tree_item.second.get("Thickness",1.0f);
                newBodyPartDef->HarvestedItem = tree_item.second.get("HarvestedItem","");
                newBodyPartDef->HarvestedQuantity = tree_item.second.get("HarvestedQuantity",1);
                newBodyPartDef->TemplateMaterialIndex = tree_item.second.get("TemplateMaterialIndex",-1);

                for (const auto& child : tree_item.second)
                {
                    if (child.first == "NaturalWeapon")
                    {
                        std::shared_ptr<NaturalWeaponDef> naturalWeaponDef=std::make_shared<NaturalWeaponDef>();
                        naturalWeaponDef->Name=child.second.get("Name","");
                        naturalWeaponDef->MaterialID=child.second.get("MaterialID","");
                        naturalWeaponDef->TemplateMaterialIndex=child.second.get("TemplateMaterialIndex",-1);
                        naturalWeaponDef->Size=child.second.get("Size",1.0f);

                        for (const auto& weapon_child : child.second)
                        {
                            if (weapon_child.first=="WeaponDef")
                            {
                                std::shared_ptr<WeaponDef> weaponDef=std::make_shared<WeaponDef>();
                                weaponDef->Skill=weapon_child.second.get("Skill","");
                                weaponDef->AmmoItemID=weapon_child.second.get("AmmoItemID","");
                                weaponDef->Point=weapon_child.second.get("Point",0.0f);
                                weaponDef->Edge=weapon_child.second.get("Edge",0.0f);
                                weaponDef->BluntModifier=weapon_child.second.get("BluntModifier",1.0f);
                                weaponDef->ProjectileModifier=weapon_child.second.get("ProjectileModifier",1.0f);
                                weaponDef->VelocityModifier=weapon_child.second.get("VelocityModifier",1.0f);
                                weaponDef->KnockbackModifier=weapon_child.second.get("KnockbackModifier",1.0f);

                                for (const auto& move_child : weapon_child.second)
                                {
                                    if (move_child.first=="AttackMove")
                                    {
                                        std::shared_ptr<AttackDef> attackDef=std::make_shared<AttackDef>();

                                        attackDef->Verb=move_child.second.get("Verb","");
                                        const std::string attackTypeStr=move_child.second.get("AttackType","");
                                        attackDef->AttackType=game::String2Enums::str2AttackType(attackTypeStr);
                                        attackDef->AttackTime=move_child.second.get("AttackTime",3.0f);
                                        attackDef->AttackRange=move_child.second.get("AttackRange",1.0f);
                                        attackDef->RequiresAmmo=move_child.second.get("RequiresAmmo",false);
                                        attackDef->Weight=move_child.second.get("Weight",1.0f);
                                        attackDef->MinimumSkillLevel=move_child.second.get("MinimumSkillLevel",1);
                                        attackDef->DamageScale=move_child.second.get("DamageScale",1.0f);

                                        std::string damageStr=move_child.second.get("Damage","");
                                        if (!damageStr.empty())
                                        {
                                            std::vector<std::string> damageSplit = split(damageStr, ",");
                                            for (auto damage : damageSplit)
                                                attackDef->DamageTypes.push_back(game::String2Enums::str2DamageType(damage));
                                        }

                                        weaponDef->AttackMoves.push_back(attackDef);
                                    }
                                    else if (move_child.first=="DefendMove")
                                    {
                                        std::shared_ptr<DefendDef> defendDef=std::make_shared<DefendDef>();
                                        defendDef->Verb=move_child.second.get("Verb","");
                                        defendDef->DefendTime=move_child.second.get("DefendTime",1.0f);
                                        defendDef->AttackDelay=move_child.second.get("AttackDelay",1.0f);

                                        std::string defendTypeStr=move_child.second.get("DefendType","");
                                        if (!defendTypeStr.empty())
                                        {
                                            std::vector<std::string> defendTypeSplit = split(defendTypeStr, ",");
                                            for (auto defend : defendTypeSplit)
                                                defendDef->DefendTypes.push_back(game::String2Enums::str2AttackType(defend));
                                        }

                                        weaponDef->DefendMoves.push_back(defendDef);
                                    }
                                    else if (move_child.first=="TargetedAttackMove")
                                    {
                                        std::shared_ptr<TargetedAttackDef> targetedAttackDef=std::make_shared<TargetedAttackDef>();

                                        std::string targetedMaterialStr=move_child.second.get("TargetedMaterial","");
                                        if (!targetedMaterialStr.empty())
                                        {
                                            std::vector<std::string> targetedMaterialSplit = split(targetedMaterialStr, ",");
                                            for (auto material : targetedMaterialSplit)
                                                targetedAttackDef->TargetedMaterials.push_back(game::String2Enums::str2MaterialType(material));
                                        }

                                        for (const auto& attack_child : move_child.second)
                                        {
                                            if (attack_child.first=="AttackDef")
                                            {
                                                std::shared_ptr<AttackDef> attackDef=std::make_shared<AttackDef>();

                                                attackDef->Verb=attack_child.second.get("Verb","");
                                                const std::string attackTypeStr=attack_child.second.get("AttackType","");
                                                attackDef->AttackType=game::String2Enums::str2AttackType(attackTypeStr);
                                                attackDef->AttackTime=attack_child.second.get("AttackTime",3.0f);
                                                attackDef->AttackRange=attack_child.second.get("AttackRange",1.0f);
                                                attackDef->RequiresAmmo=attack_child.second.get("RequiresAmmo",false);
                                                attackDef->Weight=attack_child.second.get("Weight",1.0f);
                                                attackDef->MinimumSkillLevel=attack_child.second.get("MinimumSkillLevel",1);
                                                attackDef->DamageScale=attack_child.second.get("DamageScale",1.0f);

                                                std::string damageStr=attack_child.second.get("Damage","");
                                                if (!damageStr.empty())
                                                {
                                                    std::vector<std::string> damageSplit = split(damageStr, ",");
                                                    for (auto damage : damageSplit)
                                                        attackDef->DamageTypes.push_back(game::String2Enums::str2DamageType(damage));
                                                }

                                                targetedAttackDef->TargetedAttackDef=attackDef;
                                            }
                                        }

                                        weaponDef->TargetedAttackMoves.push_back(targetedAttackDef);
                                    }
                                }

                                naturalWeaponDef->weaponDef=weaponDef;
                            }
                        }

                        newBodyPartDef->NaturalWeapon=naturalWeaponDef;
                    }
                }

                if (tree_item.second.get_child_optional("Contains"))
                {
                    for (auto const &contains : tree_item.second.get_child("Contains"))
                    {
                        if (contains.first=="Item")
                        {
                            std::string containsID=contains.second.get_value<std::string>();
                            auto containsPtr=bodyPartDefinition(containsID);
                            if (containsPtr!=nullptr)
                                newBodyPartDef->ContainedParts.push_back(containsPtr);
                        }
                    }
                }

                mBodyPartDefs.push_back(newBodyPartDef);
            }
        }
    }

    void GameDefinition::parseBodyDefs()
    {
        std::string path="../settings/objects/body.xml";

        boost::property_tree::ptree pt;
        boost::property_tree::read_xml(path, pt);

        std::vector<std::shared_ptr<BodySectionDef>> body_sections_container;

        for (auto const& tree_item : pt.get_child("Bodies"))
        {
            if (tree_item.first == "Item")
            {
                if (tree_item.second.get_child_optional("MainBody"))
                {
                    std::shared_ptr<BodyDef> newBodyDef=std::make_shared<BodyDef>();
                    newBodyDef->ID = tree_item.second.get("ID", "");

                    for (auto const &contains : tree_item.second.get_child("MainBody"))
                    {
                        if (contains.first == "Item")
                        {
                            std::string mainBodyID = contains.second.get_value<std::string>();
                            auto iter = std::find_if(body_sections_container.begin(), body_sections_container.end(),
                                                     [&mainBodyID](std::shared_ptr<BodySectionDef> const &item)
                                                     {
                                                         return item->ID == mainBodyID;
                                                     });

                            if (iter != body_sections_container.end())
                            {
                                newBodyDef->MainBody=(*iter);
                            }
                        }
                    }

                    mBodyDefs.push_back(newBodyDef);
                    body_sections_container.clear();
                }
                else
                    {
                    std::shared_ptr<BodySectionDef> newBodySectionDef = std::make_shared<BodySectionDef>();

                    newBodySectionDef->ID = tree_item.second.get("ID", "");
                    newBodySectionDef->Name = tree_item.second.get("Name", "");
                    newBodySectionDef->BodyPartID = tree_item.second.get("BodyPartID", "");
                    newBodySectionDef->MaterialID = tree_item.second.get("MaterialID", "");
                    newBodySectionDef->ToHitWeight = tree_item.second.get("ToHitWeight", 0.0f);

                    std::string equipType = tree_item.second.get("EquipType", "");
                    newBodySectionDef->EquipType = game::String2Enums::str2EquipmentType(equipType);

                    newBodySectionDef->Symmetrical = tree_item.second.get("Symmetrical", false);
                    newBodySectionDef->Limb = tree_item.second.get("Limb", false);
                    newBodySectionDef->Connection = tree_item.second.get("Connection", false);
                    newBodySectionDef->Hand = tree_item.second.get("Hand", false);

                    newBodySectionDef->SpriteID = tree_item.second.get("SpriteID", "");
                    newBodySectionDef->SpriteIDRight = tree_item.second.get("SpriteIDRight", "");
                    newBodySectionDef->SpriteIDLeft = tree_item.second.get("SpriteIDLeft", "");
                    newBodySectionDef->DrawOrder = tree_item.second.get("DrawOrder", -1);
                    newBodySectionDef->TemplateMaterialIndex = tree_item.second.get("TemplateMaterialIndex", -1);

                    if (tree_item.second.get_child_optional("Decoration"))
                    {
                        auto decoration_tree=tree_item.second.get_child("Decoration");
                        for (auto const &contains : decoration_tree)
                        {
                            std::string spriteID = contains.second.get_value<std::string>();
                            if (contains.first == "SpriteID")
                            {
                                newBodySectionDef->Decorations.push_back(spriteID);
                            }
                            else if (contains.first == "SpriteIDRight")
                            {
                                newBodySectionDef->DecorationsRight.push_back(spriteID);
                            }
                            else if (contains.first == "SpriteIDLeft")
                            {
                                newBodySectionDef->DecorationsLeft.push_back(spriteID);
                            }
                        }

                        if (decoration_tree.get_child_optional("Additional"))
                        {
                            for (auto const &contains : decoration_tree.get_child("Additional"))
                            {
                                if ((contains.first == "Male") || (contains.first == "Female"))
                                {
                                    std::vector<std::string> container;
                                    for (auto const item : contains.second)
                                    {
                                        std::string spriteID = item.second.get_value<std::string>();
                                        container.push_back(spriteID);
                                    }

                                    if (contains.first == "Male")
                                        newBodySectionDef->AdditionalDecorationMale.push_back(container);
                                    else if (contains.first == "Female")
                                        newBodySectionDef->AdditionalDecorationFemale.push_back(container);
                                }
                            }
                        }
                    }

                    if (tree_item.second.get_child_optional("ConnectsTo"))
                    {
                        for (auto const &contains : tree_item.second.get_child("ConnectsTo"))
                        {
                            if (contains.first == "Item")
                            {
                                std::string connectsToID = contains.second.get_value<std::string>();
                                auto iter = std::find_if(body_sections_container.begin(), body_sections_container.end(),
                                                         [&connectsToID]
                                                                 (std::shared_ptr<BodySectionDef> const &item) {
                                                             return item->ID == connectsToID;
                                                         });

                                if (iter != body_sections_container.end())
                                {
                                    newBodySectionDef->ConnectedSections.push_back((*iter));
                                }
                            }
                        }
                    }

                    body_sections_container.push_back(newBodySectionDef);
                }
            }
        }
    }

    int GameDefinition::playerFactionIndex() const
    {
        return mPlayerFactionIndex;
    }

    std::shared_ptr<const FactionDef> GameDefinition::factionDef(const int& index) const
    {
        return mFactionDefs.at(index);
    }

    std::shared_ptr<const FactionDef> GameDefinition::factionDef(const std::string& factionID) const
    {
        const auto iter=std::find_if(mFactionDefs.begin(),mFactionDefs.end(),[&factionID](std::shared_ptr<FactionDef> const& factionElem)
        {
           return factionID==factionElem->ID;
        });

        if (iter!=mFactionDefs.end())
            return (*iter);
        else
            return nullptr;
    }

    std::vector<std::shared_ptr<const FactionDef>> GameDefinition::factionDefs(const game::FactionType& type) const
    {
        std::vector<std::shared_ptr<const FactionDef>> result;
        for (const auto& factionDef : mFactionDefs)
        {
            if (factionDef->Type==type)
            {
                result.push_back(factionDef);
            }
        }

        return result;
    }

    std::shared_ptr<const ItemSettings> GameDefinition::itemSettings() const
    {
        return mItemSettings;
    }

    std::shared_ptr<const UniformSettings> GameDefinition::uniformSettings() const
    {
        return mUniformSettings;
    }

    std::shared_ptr<const AmmoDef> GameDefinition::ammoDefinition(const std::string& id) const
    {
        auto iter=std::find_if(mAmmoDefs.begin(),mAmmoDefs.end(),[&id](std::shared_ptr<AmmoDef> const& value)
        {
            return (id==value->AmmoID);
        });

        if (iter!=mAmmoDefs.end())
            return (*iter);
        else
            return nullptr;
    }

    std::string GameDefinition::itemName(const std::string& itemID,const std::string& materialID,std::shared_ptr<const game::Item> item)
    {
        auto itemDef=GAME_DEFINITIONS->itemDefinition(itemID);
        auto materialDef=GAME_DEFINITIONS->materialDefinition(materialID);

        std::string result;
        if (!itemDef->Prefix.empty())
            result+=itemDef->Prefix+" ";

        if (item!=nullptr)
            result+=item->materialName()+" ";
        else if (!materialDef->GroupName.empty())
            result+=materialDef->GroupName+" ";
        else
            result+=materialDef->Name+" ";

        if (itemDef!=nullptr)
            result+=itemDef->Name+" ";

        if (!itemDef->Suffix.empty())
            result+=itemDef->Suffix+" ";

        return result;
    }

    std::shared_ptr<const CharacterSettings> GameDefinition::characterSettings() const
    {
        return mCharacterSettings;
    }

    void GameDefinition::parseRaceDefs()
    {
        std::string path="../settings/objects/race.xml";

        boost::property_tree::ptree pt;
        boost::property_tree::read_xml(path, pt);

        for (auto const& tree_item : pt.get_child("Races"))
        {
            if (tree_item.first == "Item")
            {
                std::shared_ptr<RaceDefinition> raceDefinition=std::make_shared<RaceDefinition>();
                raceDefinition->ID=tree_item.second.get("ID","");
                raceDefinition->Name=tree_item.second.get("Name","");
                raceDefinition->MoveSpeed=tree_item.second.get("MoveSpeed",1.0f);
                raceDefinition->Size=tree_item.second.get("Size",10.0f);
                raceDefinition->CastLightRadius=tree_item.second.get("CastLightRadius",8.0f);
                raceDefinition->SightRadius=tree_item.second.get("SightRadius",16.0f);
                raceDefinition->CombatSightRadius=tree_item.second.get("CombatSightRadius",16.0f);
                raceDefinition->ExhaustionTime=tree_item.second.get("ExhaustionTime",1.0f);
                raceDefinition->RestTime=tree_item.second.get("RestTime",1.0f/3.0f);
                raceDefinition->TiredLevel=tree_item.second.get("TiredLevel",100.0f/3.0f);
                raceDefinition->PassOutLevel=tree_item.second.get("PassOutLevel",16.0f);
                raceDefinition->HungerRate=tree_item.second.get("HungerRate",2.0f);
                raceDefinition->FoodRatio=tree_item.second.get("FoodRatio",1.0f);
                raceDefinition->HungerLevel=tree_item.second.get("HungerLevel",100.0f);
                raceDefinition->StarvationLevel=tree_item.second.get("StarvationLevel",75.0f);
                raceDefinition->ThirstRate=tree_item.second.get("ThirstRate",1.5f);
                raceDefinition->DrinkRatio=tree_item.second.get("DrinkRatio",1.0f);
                raceDefinition->ThirstLevel=tree_item.second.get("ThirstLevel",85.0f);
                raceDefinition->DyingOfThirstLevel=tree_item.second.get("DyingOfThirstLevel",50.0f);
                raceDefinition->BloodLossRate=tree_item.second.get("BloodLossRate",1.0f);
                raceDefinition->HealingItemID=tree_item.second.get("HealingItemID","Bandage");
                raceDefinition->ZombieVirusCarrier=tree_item.second.get("ZombieVirusCarrier",false);
                raceDefinition->Livestock=tree_item.second.get("Livestock",false);
                raceDefinition->PastureSpace=tree_item.second.get("PastureSpace",4);
                raceDefinition->GestationTimeMin=tree_item.second.get("GestationTimeMin",0.0f);
                raceDefinition->GestationTimeMax=tree_item.second.get("GestationTimeMax",0.0f);
                raceDefinition->BaseAttackDelay=tree_item.second.get("BaseAttackDelay",1.0f);
                raceDefinition->DodgeTime=tree_item.second.get("DodgeTime",1.0f);
                raceDefinition->BodyID=tree_item.second.get("BodyID","");
                raceDefinition->LanguageID=tree_item.second.get("LanguageID","");

                for (auto const &child : tree_item.second)
                {
                    if (child.first == "Gender")
                    {
                        std::shared_ptr<properties::GenderDef> genderDef=std::make_shared<properties::GenderDef>();
                        const std::string genderStr=child.second.get("Gender","");
                        genderDef->Gender=game::String2Enums::str2GenderType(genderStr);
                        genderDef->Name=child.second.get("Name","");
                        genderDef->RandomWeight=child.second.get("RandomWeight",1.0f);
                        raceDefinition->Genders.push_back(genderDef);
                    }
                    else if (child.first == "Attribute")
                    {
                        properties::AttributeDef attributeDef;

                        const std::string typeStr=child.second.get("Attribute","");
                        attributeDef.Attribute=game::String2Enums::str2CharacterAttributeType(typeStr);
                        attributeDef.Min=child.second.get("Min",0);
                        attributeDef.Max=child.second.get("Max",0);
                        attributeDef.Mean=child.second.get("Mean",0);
                        raceDefinition->Attributes.push_back(attributeDef);
                    }
                    else if (child.first == "AdditionalDiet")
                    {
                        const std::string key=child.second.get("Key","");
                        const float value=child.second.get("Value",0.0f);

                        if ((!key.empty()) && (value>0.0f))
                            raceDefinition->AdditionalDiet.push_back({key,value});
                    }
                }

                mRaceDefinitions.push_back(raceDefinition);
            }
        }
    }

    void GameDefinition::parseFactionDefs()
    {
        std::string path="../settings/objects/faction.xml";

        boost::property_tree::ptree pt;
        boost::property_tree::read_xml(path, pt);

        for (auto const& tree_item : pt.get_child("Factions"))
        {
            if (tree_item.first == "Item")
            {
                std::shared_ptr<FactionDef> factionDef = std::make_shared<FactionDef>();
                factionDef->ID = tree_item.second.get("ID", "");
                const std::string typeStr = tree_item.second.get("Type", "");
                factionDef->Type=game::String2Enums::str2FactionType(typeStr);
                factionDef->LanguageID = tree_item.second.get("LanguageID", "");
                factionDef->SubType = tree_item.second.get("SubType", "");
                factionDef->Description = tree_item.second.get("Description", "");

                for (auto const &child : tree_item.second)
                {
                    if (child.first == "Squad")
                    {
                        std::shared_ptr<SquadDef> squadDef = std::make_shared<SquadDef>();
                        squadDef->BaseCombatValue = child.second.get("BaseCombatValue", 1.0f);
                        const std::string typeStr = child.second.get("Type", "Attack");
                        squadDef->Type = game::String2Enums::str2SquadType(typeStr);
                        squadDef->AllClasses = child.second.get("AllClasses", true);

                        for (auto const &class_child : child.second)
                        {
                            if (class_child.first == "Class")
                            {
                                std::shared_ptr<RaceClassDef> raceClassDef = std::make_shared<RaceClassDef>();
                                raceClassDef->RaceID=class_child.second.get("RaceID", "");

                                squadDef->Classes.push_back(raceClassDef);
                            }
                        }

                        factionDef->Squads.push_back(squadDef);
                    }
                }

                if (factionDef->Type==game::FactionType::PlayerCiv)
                    mPlayerFactionIndex=mFactionDefs.size();

                mFactionDefs.push_back(factionDef);
            }
        }
    }
}