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
#include "game/core/String2Enums.h"
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
        mTerrainSettings=std::shared_ptr<TerrainSettings>(new TerrainSettings);
        mTerrainSettings->AirMaterialID="Air";
        mTerrainSettings->parseTerrainSettingsFile();

        parseMaterialDefsFile();

        parseSpriteDefsFile();

        std::shared_ptr<RaceDefinition> settlerDefinition=std::shared_ptr<RaceDefinition>(new RaceDefinition());
        settlerDefinition->ID="Gnome";
        settlerDefinition->Name="gnome";
        settlerDefinition->BodyID="Gnome";
        settlerDefinition->MoveSpeed=8.0f;
        settlerDefinition->TiredLevel=33.0f;
        settlerDefinition->ThirstRate=1.5f;
        settlerDefinition->ThirstLevel=84.0f;
        settlerDefinition->DyingOfThirstLevel=50.0f;
        settlerDefinition->DrinkRatio=1.0f;
        settlerDefinition->BloodLossRate=1.0f;
        settlerDefinition->RestTime=0.334f;
        settlerDefinition->ExhaustionTime=1.0f;
        settlerDefinition->HungerRate=2.0f;
        settlerDefinition->StarvationLevel=75.0f;
        settlerDefinition->HungerLevel=100.0f;
        settlerDefinition->FoodRatio=1.0f;
        settlerDefinition->Attributes.push_back(properties::AttributeDef(game::CharacterAttributeType::Fitness,80,120,100));
        settlerDefinition->Attributes.push_back(properties::AttributeDef(game::CharacterAttributeType::Nimbleness,80,120,100));
        settlerDefinition->Attributes.push_back(properties::AttributeDef(game::CharacterAttributeType::Curiosity,80,120,100));
        settlerDefinition->Attributes.push_back(properties::AttributeDef(game::CharacterAttributeType::Focus,80,120,100));
        settlerDefinition->Attributes.push_back(properties::AttributeDef(game::CharacterAttributeType::Charm,80,120,100));
        settlerDefinition->Genders.push_back({game::GenderType::Male,1.0f});
        settlerDefinition->Genders.push_back({game::GenderType::Female,1.0f});
        mRaceDefinitions.push_back(settlerDefinition);

        std::shared_ptr<ConstructionDef> soilStairs=std::shared_ptr<ConstructionDef>(new ConstructionDef());
        soilStairs->ID="SoilStairs";
        soilStairs->Name="stairs";
        soilStairs->Prefix="soil";
        mConstructionDefs.push_back(soilStairs);

        std::shared_ptr<ConstructionDef> rawStoneRamp=std::shared_ptr<ConstructionDef>(new ConstructionDef());
        rawStoneRamp->ID="RawStoneRamp";
        rawStoneRamp->Name="ramp";
        rawStoneRamp->Prefix="stone";
        mConstructionDefs.push_back(rawStoneRamp);

        parseItemDefs();

        mGameSettings=std::shared_ptr<GameSettings>(new GameSettings());
        mGameSettings->NutritionWeight=50.0f;
        mGameSettings->DepthDistanceBias=1.0f;

        parseBodyPartDefs();
        //this->initBodyPartDefs();
        parseBodyDefs();
        //this->initBodyDefs();
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
        auto iter=std::find_if(std::begin(mMaterialDefs),std::end(mMaterialDefs),[&id](std::shared_ptr<MaterialDef> const& value)
        {
           return (id==value->ID);
        });

        if (iter==std::end(mMaterialDefs))
            return -1;
        else
            return std::distance(mMaterialDefs.begin(),iter);
    }

    std::shared_ptr<const RaceDefinition> GameDefinition::raceDefinition(const std::string& id) const
    {
        auto iter=std::find_if(std::begin(mRaceDefinitions),std::end(mRaceDefinitions),[&id](std::shared_ptr<RaceDefinition> const& value)
        {
            return (id==value->ID);
        });

        if (iter!=std::end(mRaceDefinitions))
        {
            int index=std::distance(mRaceDefinitions.begin(),iter);
            return mRaceDefinitions.at(index);
        }
        else
            return nullptr;
    }

    std::shared_ptr<const ConstructionDef> GameDefinition::constructionDefinition(const std::string& id) const
    {
        auto iter=std::find_if(std::begin(mConstructionDefs),std::end(mConstructionDefs),[&id](std::shared_ptr<ConstructionDef> const& value)
        {
            return (id==value->ID);
        });

        if (iter!=std::end(mConstructionDefs))
        {
            int index=std::distance(mConstructionDefs.begin(),iter);
            return mConstructionDefs.at(index);
        }
        else
            return nullptr;
    }

    std::shared_ptr<const ItemDefinition> GameDefinition::itemDefinition(const std::string& id) const
    {
        auto iter=std::find_if(std::begin(mItemDefinitions),std::end(mItemDefinitions),[&id](std::shared_ptr<ItemDefinition> const& value)
        {
            return (id==value->ID);
        });

        if (iter!=std::end(mItemDefinitions))
        {
            int index=std::distance(mItemDefinitions.begin(),iter);
            return mItemDefinitions.at(index);
        }
        else
            return nullptr;
    }

    std::shared_ptr<const GameSettings> GameDefinition::gameSettings() const
    {
        return mGameSettings;
    }

    std::shared_ptr<const BodyPartDef> GameDefinition::bodyPartDefinition(const std::string& id) const
    {
        auto iter=std::find_if(std::begin(mBodyPartDefs),std::end(mBodyPartDefs),[&id](std::shared_ptr<BodyPartDef> const& value)
        {
            return (id==value->ID);
        });

        if (iter!=std::end(mBodyPartDefs))
        {
            int index=std::distance(mBodyPartDefs.begin(),iter);
            return mBodyPartDefs.at(index);
        }
        else
            return nullptr;
    }

    void GameDefinition::initBodyPartDefs()
    {
        //Gnome parts
        {
            std::shared_ptr<BodyPartDef> lung = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            lung->Name = "lung";
            lung->MaterialID = game::Material::StandardWeakFlesh;
            lung->BodyFunction = game::BodyFunction::Breathe;
            lung->Symmetrical = true;

            std::shared_ptr<BodyPartDef> heart = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            heart->Name = "heart";
            heart->MaterialID = game::Material::StandardWeakFlesh;
            heart->BodyFunction = game::BodyFunction::Circulation;
            heart->ToHitWeight = 0.5f;

            std::shared_ptr<BodyPartDef> ribs = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            ribs->Name = "ribs";
            ribs->MaterialID = game::Material::StandardBone;
            ribs->BodyProperties.push_back(game::BodyPartProperty::Splinters);
            ribs->ContainedParts.push_back(lung);
            ribs->ContainedParts.push_back(heart);

            std::shared_ptr<BodyPartDef> guts = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            guts->Name = "guts";
            guts->MaterialID = game::Material::StandardWeakFlesh;
            guts->BodyFunction = game::BodyFunction::InternalOrgan;
            guts->BodyProperties.push_back(game::BodyPartProperty::HasBlood);
            guts->BodyProperties.push_back(game::BodyPartProperty::HasArtery);
            guts->BodyProperties.push_back(game::BodyPartProperty::Nauseates);

            std::shared_ptr<BodyPartDef> abdominalMuscles = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            abdominalMuscles->Name = "abdominal muscles";
            abdominalMuscles->MaterialID = game::Material::StandardStrongFlesh;
            abdominalMuscles->BodyProperties.push_back(game::BodyPartProperty::HasBlood);
            abdominalMuscles->BodyProperties.push_back(game::BodyPartProperty::HasArtery);
            abdominalMuscles->ContainedParts.push_back(guts);

            std::shared_ptr<BodyPartDef> brain = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            brain->Name = "brain";
            brain->MaterialID = game::Material::StandardWeakFlesh;
            brain->BodyFunction = game::BodyFunction::Thought;

            std::shared_ptr<BodyPartDef> skull = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            skull->Name = "skull";
            skull->MaterialID = game::Material::StandardShell;
            skull->BodyFunction = game::BodyFunction::Structure;
            skull->HarvestedItem = "Skull";
            skull->HarvestedQuantity = 1;
            skull->ContainedParts.push_back(brain);

            std::shared_ptr<BodyPartDef> throat = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            throat->Name = "throat";
            throat->MaterialID = game::Material::StandardWeakFlesh;
            throat->BodyFunction = game::BodyFunction::Throat;

            std::shared_ptr<BodyPartDef> boneForArm = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            boneForArm->Name = "bone";
            boneForArm->MaterialID = game::Material::StandardBone;
            boneForArm->BodyFunction = game::BodyFunction::Structure;

            std::shared_ptr<BodyPartDef> muscleForArm = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            muscleForArm->Name = "muscle";
            muscleForArm->MaterialID = game::Material::StandardStrongFlesh;
            muscleForArm->BodyFunction = game::BodyFunction::Motor;
            muscleForArm->BodyProperties.push_back(game::BodyPartProperty::HasBlood);
            muscleForArm->BodyProperties.push_back(game::BodyPartProperty::HasArtery);
            muscleForArm->HarvestedItem = "Giblets";
            muscleForArm->HarvestedQuantity = 1;
            muscleForArm->ContainedParts.push_back(boneForArm);

            std::shared_ptr<BodyPartDef> boneForLeg = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            boneForLeg->Name = "bone";
            boneForLeg->MaterialID = game::Material::StandardBone;
            boneForLeg->BodyFunction = game::BodyFunction::Structure;
            boneForLeg->HarvestedItem = "Bone";
            boneForLeg->HarvestedQuantity = 1;

            std::shared_ptr<BodyPartDef> muscleForLeg = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            muscleForLeg->Name = "muscle";
            muscleForLeg->MaterialID = game::Material::StandardStrongFlesh;
            muscleForLeg->BodyFunction = game::BodyFunction::Motor;
            muscleForLeg->BodyProperties.push_back(game::BodyPartProperty::HasBlood);
            muscleForLeg->BodyProperties.push_back(game::BodyPartProperty::HasArtery);
            muscleForLeg->HarvestedItem = "Giblets";
            muscleForLeg->HarvestedQuantity = 1;
            muscleForLeg->ContainedParts.push_back(boneForLeg);

            std::shared_ptr<BodyPartDef> muscleForHand = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            muscleForHand->Name = "muscle";
            muscleForHand->MaterialID = game::Material::StandardStrongFlesh;
            muscleForHand->BodyFunction = game::BodyFunction::Grip;
            muscleForHand->BodyProperties.push_back(game::BodyPartProperty::HasBlood);

            std::shared_ptr<BodyPartDef> boneForHand = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            boneForHand->Name = "bone";
            boneForHand->MaterialID = game::Material::StandardBone;
            boneForHand->BodyFunction = game::BodyFunction::Structure;
            boneForHand->BodyProperties.push_back(game::BodyPartProperty::Splinters);
            boneForHand->ContainedParts.push_back(muscleForHand);

            std::shared_ptr<BodyPartDef> muscleForFoot = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            muscleForFoot->Name = "muscle";
            muscleForFoot->MaterialID = game::Material::StandardStrongFlesh;
            muscleForFoot->BodyFunction = game::BodyFunction::Stand;
            muscleForFoot->BodyProperties.push_back(game::BodyPartProperty::HasBlood);

            std::shared_ptr<BodyPartDef> boneForFoot = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            boneForFoot->Name = "bone";
            boneForFoot->MaterialID = game::Material::StandardBone;
            boneForFoot->BodyFunction = game::BodyFunction::Structure;
            boneForFoot->BodyProperties.push_back(game::BodyPartProperty::Splinters);
            boneForFoot->ContainedParts.push_back(muscleForFoot);

            std::shared_ptr<BodyPartDef> gnomeUpperBody = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            gnomeUpperBody->ID = "GnomeUpperBody";
            gnomeUpperBody->Name = "skin";
            gnomeUpperBody->MaterialID = game::Material::StandardSkin;
            gnomeUpperBody->BodyProperties.push_back(game::BodyPartProperty::HasBlood);
            gnomeUpperBody->ContainedParts.push_back(ribs);
            mBodyPartDefs.push_back(gnomeUpperBody);

            std::shared_ptr<BodyPartDef> gnomeLowerBody = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            gnomeLowerBody->ID = "GnomeLowerBody";
            gnomeLowerBody->Name = "skin";
            gnomeLowerBody->MaterialID = game::Material::StandardSkin;
            gnomeLowerBody->BodyProperties.push_back(game::BodyPartProperty::HasBlood);
            gnomeLowerBody->ContainedParts.push_back(abdominalMuscles);
            mBodyPartDefs.push_back(gnomeLowerBody);

            std::shared_ptr<BodyPartDef> gnomeHead = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            gnomeHead->ID = "GnomeHead";
            gnomeHead->Name = "skin";
            gnomeHead->MaterialID = game::Material::StandardSkin;
            gnomeHead->BodyProperties.push_back(game::BodyPartProperty::HasBlood);
            gnomeHead->ContainedParts.push_back(skull);
            mBodyPartDefs.push_back(gnomeHead);

            std::shared_ptr<BodyPartDef> gnomeEye = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            gnomeEye->ID = "GnomeEye";
            gnomeEye->Name = "eye";
            gnomeEye->MaterialID = game::Material::StandardWeakFlesh;
            gnomeEye->BodyFunction = game::BodyFunction::Sight;
            gnomeEye->Symmetrical = true;
            mBodyPartDefs.push_back(gnomeEye);

            std::shared_ptr<BodyPartDef> gnomeMouth = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            gnomeMouth->ID = "GnomeMouth";
            gnomeMouth->Name = "jaw";
            gnomeMouth->MaterialID = game::Material::StandardBone;
            mBodyPartDefs.push_back(gnomeMouth);

            std::shared_ptr<BodyPartDef> gnomeNeck = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            gnomeNeck->ID = "GnomeNeck";
            gnomeNeck->Name = "skin";
            gnomeNeck->MaterialID = game::Material::StandardSkin;
            gnomeNeck->BodyProperties.push_back(game::BodyPartProperty::HasBlood);
            gnomeNeck->BodyProperties.push_back(game::BodyPartProperty::HasArtery);
            gnomeNeck->ContainedParts.push_back(throat);
            mBodyPartDefs.push_back(gnomeNeck);

            std::shared_ptr<BodyPartDef> gnomeArm = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            gnomeArm->ID = "GnomeArm";
            gnomeArm->Name = "skin";
            gnomeArm->MaterialID = game::Material::StandardSkin;
            gnomeArm->BodyProperties.push_back(game::BodyPartProperty::HasBlood);
            gnomeArm->Symmetrical = true;
            gnomeArm->ContainedParts.push_back(muscleForArm);
            mBodyPartDefs.push_back(gnomeArm);

            std::shared_ptr<BodyPartDef> gnomeLeg = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            gnomeLeg->ID = "GnomeLeg";
            gnomeLeg->Name = "skin";
            gnomeLeg->MaterialID = game::Material::StandardSkin;
            gnomeLeg->BodyProperties.push_back(game::BodyPartProperty::HasBlood);
            gnomeLeg->Symmetrical = true;
            gnomeLeg->ContainedParts.push_back(muscleForLeg);
            mBodyPartDefs.push_back(gnomeLeg);

            std::shared_ptr<BodyPartDef> gnomeHand = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            gnomeHand->ID = "GnomeHand";
            gnomeHand->Name = "skin";
            gnomeHand->MaterialID = game::Material::StandardSkin;
            gnomeHand->BodyProperties.push_back(game::BodyPartProperty::HasBlood);
            gnomeHand->ContainedParts.push_back(boneForHand);
            mBodyPartDefs.push_back(gnomeHand);

            std::shared_ptr<BodyPartDef> gnomeFoot = std::shared_ptr<BodyPartDef>(new BodyPartDef());
            gnomeFoot->ID = "GnomeFoot";
            gnomeFoot->Name = "skin";
            gnomeFoot->MaterialID = game::Material::StandardSkin;
            gnomeFoot->BodyProperties.push_back(game::BodyPartProperty::HasBlood);
            gnomeFoot->ContainedParts.push_back(boneForFoot);
            mBodyPartDefs.push_back(gnomeFoot);
        }
    }

    std::shared_ptr<const BodyDef> GameDefinition::bodyDefinition(const std::string& id) const
    {
        auto iter=std::find_if(std::begin(mBodyDefs),std::end(mBodyDefs),[&id](std::shared_ptr<BodyDef> const& value)
        {
            return (id==value->ID);
        });

        if (iter!=std::end(mBodyDefs))
        {
            int index=std::distance(mBodyDefs.begin(),iter);
            return mBodyDefs.at(index);
        }
        else
            return nullptr;
    }

    void GameDefinition::initBodyDefs()
    {
        std::shared_ptr<BodySectionDef> gnomeFoot=std::shared_ptr<BodySectionDef>(new BodySectionDef());
        gnomeFoot->Name="foot";
        gnomeFoot->BodyPartID="GnomeFoot";
        gnomeFoot->MaterialID=game::Material::StandardBone;
        gnomeFoot->ToHitWeight=0.1f;
        gnomeFoot->EquipType=game::EquipmentType::Foot;

        std::shared_ptr<BodySectionDef> gnomeLeg=std::shared_ptr<BodySectionDef>(new BodySectionDef());
        gnomeLeg->Name="leg";
        gnomeLeg->BodyPartID="GnomeLeg";
        gnomeLeg->MaterialID=game::Material::StandardBone;
        gnomeLeg->ToHitWeight=0.5f;
        gnomeLeg->Symmetrical=true;
        gnomeLeg->EquipType=game::EquipmentType::Leg;
        gnomeLeg->ConnectedSections.push_back(gnomeFoot);

        std::shared_ptr<BodySectionDef> gnomeLowerBody=std::shared_ptr<BodySectionDef>(new BodySectionDef());
        gnomeLowerBody->Name="lower body";
        gnomeLowerBody->BodyPartID="GnomeLowerBody";
        gnomeLowerBody->MaterialID=game::Material::StandardBone;
        gnomeLowerBody->ToHitWeight=0.75f;
        gnomeLowerBody->Limb=false;
        gnomeLowerBody->EquipType=game::EquipmentType::Body;
        gnomeLowerBody->ConnectedSections.push_back(gnomeLeg);

        std::shared_ptr<BodySectionDef> gnomeEye=std::shared_ptr<BodySectionDef>(new BodySectionDef());
        gnomeEye->Name="eye";
        gnomeEye->BodyPartID="GnomeEye";
        gnomeEye->MaterialID=game::Material::StandardWeakFlesh;
        gnomeEye->ToHitWeight=0.05f;
        gnomeEye->Limb=false;
        gnomeEye->Symmetrical=true;
        gnomeEye->EquipType=game::EquipmentType::Head;

        std::shared_ptr<BodySectionDef> gnomeMouth=std::shared_ptr<BodySectionDef>(new BodySectionDef());
        gnomeMouth->Name="mouth";
        gnomeMouth->BodyPartID="GnomeMouth";
        gnomeMouth->MaterialID=game::Material::StandardWeakFlesh;
        gnomeMouth->ToHitWeight=0.1f;
        gnomeMouth->Limb=false;
        gnomeMouth->EquipType=game::EquipmentType::Head;

        std::shared_ptr<BodySectionDef> gnomeHead=std::shared_ptr<BodySectionDef>(new BodySectionDef());
        gnomeHead->Name="head";
        gnomeHead->BodyPartID="GnomeHead";
        gnomeHead->MaterialID=game::Material::StandardShell;
        gnomeHead->ToHitWeight=0.2f;
        gnomeHead->Limb=false;
        gnomeHead->EquipType=game::EquipmentType::Head;
        gnomeHead->ConnectedSections.push_back(gnomeEye);
        gnomeHead->ConnectedSections.push_back(gnomeMouth);

        std::shared_ptr<BodySectionDef> gnomeNeck=std::shared_ptr<BodySectionDef>(new BodySectionDef());
        gnomeNeck->Name="neck";
        gnomeNeck->BodyPartID="GnomeNeck";
        gnomeNeck->MaterialID=game::Material::StandardWeakFlesh;
        gnomeNeck->ToHitWeight=0.05f;
        gnomeNeck->Connection=true;
        gnomeNeck->EquipType=game::EquipmentType::Body;
        gnomeNeck->ConnectedSections.push_back(gnomeHead);

        std::shared_ptr<BodySectionDef> gnomeHand=std::shared_ptr<BodySectionDef>(new BodySectionDef());
        gnomeHand->Name="hand";
        gnomeHand->BodyPartID="GnomeHand";
        gnomeHand->MaterialID=game::Material::StandardBone;
        gnomeHand->ToHitWeight=0.1f;
        gnomeHand->EquipType=game::EquipmentType::Glove;
        gnomeHand->Hand=true;

        std::shared_ptr<BodySectionDef> gnomeArm=std::shared_ptr<BodySectionDef>(new BodySectionDef());
        gnomeArm->Name="arm";
        gnomeArm->BodyPartID="GnomeArm";
        gnomeArm->MaterialID=game::Material::StandardBone;
        gnomeArm->ToHitWeight=0.5f;
        gnomeArm->Symmetrical=true;
        gnomeArm->EquipType=game::EquipmentType::Arm;
        gnomeArm->ConnectedSections.push_back(gnomeHand);

        std::shared_ptr<BodySectionDef> gnomeUpperBody=std::shared_ptr<BodySectionDef>(new BodySectionDef());
        gnomeUpperBody->Name="upper body";
        gnomeUpperBody->BodyPartID="GnomeUpperBody";
        gnomeUpperBody->MaterialID=game::Material::StandardBone;
        gnomeUpperBody->ToHitWeight=1.0f;
        gnomeUpperBody->Limb=false;
        gnomeUpperBody->EquipType=game::EquipmentType::Body;
        gnomeUpperBody->ConnectedSections.push_back(gnomeLowerBody);
        gnomeUpperBody->ConnectedSections.push_back(gnomeNeck);
        gnomeUpperBody->ConnectedSections.push_back(gnomeArm);

        std::shared_ptr<BodyDef> gnomeBody=std::shared_ptr<BodyDef>(new BodyDef());
        gnomeBody->ID="Gnome";
        gnomeBody->MainBody=gnomeUpperBody;
        mBodyDefs.push_back(gnomeBody);
    }

    std::shared_ptr<const MaterialDef> GameDefinition::materialDefinition(const std::string& id) const
    {
        auto iter=std::find_if(std::begin(mMaterialDefs),std::end(mMaterialDefs),[&id](std::shared_ptr<MaterialDef> const& value)
        {
            return (id==value->ID);
        });

        if (iter!=std::end(mMaterialDefs))
        {
            int index=std::distance(mMaterialDefs.begin(),iter);
            return mMaterialDefs.at(index);
        }
        else
            return nullptr;
    }

    std::shared_ptr<const MaterialDef> GameDefinition::materialDefinition(const int& index) const
    {
        return mMaterialDefs.at(index);
    }

    void GameDefinition::parseMaterialDefsFile()
    {
        std::string path="../settings/objects/material.xml";

        boost::property_tree::ptree pt;
        boost::property_tree::read_xml(path, pt);

        for (auto const& tree_item : pt.get_child("Materials"))
        {
            if (tree_item.first=="Item")
            {
                std::shared_ptr<MaterialDef> newMaterialDef=std::shared_ptr<MaterialDef>(new MaterialDef());
                newMaterialDef->ID=tree_item.second.get("ID","");
                newMaterialDef->Name=tree_item.second.get("Name","");
                auto typeStr=tree_item.second.get("Type","");
                newMaterialDef->Type=game::String2Enums::str2MaterialType(typeStr);
                newMaterialDef->Strength=tree_item.second.get("Strength",0.0f);
                newMaterialDef->Value=tree_item.second.get("Value",0.0f);
                std::string colorStr=tree_item.second.get("Color","");
                newMaterialDef->Color=gui::ZColor(colorStr);
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

        for (auto const& tree_item : pt.get_child("Sprites"))
        {
            if (tree_item.first=="Item")
            {
                std::shared_ptr<SpriteDef> newSpriteDef=std::shared_ptr<SpriteDef>(new SpriteDef());
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
        {
            int index=std::distance(mSpriteDefs.begin(),iter);
            return mSpriteDefs.at(index);
        }
        else
            return nullptr;
    }

    void GameDefinition::parseItemDefs()
    {
        std::string path="../settings/objects/items.xml";

        boost::property_tree::ptree pt;
        boost::property_tree::read_xml(path, pt);

        for (auto const& tree_item : pt.get_child("Items"))
        {
            if (tree_item.first=="Item")
            {
                std::shared_ptr<ItemDefinition> newItemDefinition=std::shared_ptr<ItemDefinition>(new ItemDefinition());
                newItemDefinition->ID=tree_item.second.get("ID","");
                newItemDefinition->Name=tree_item.second.get("Name","");

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
                std::shared_ptr<BodyPartDef> newBodyPartDef=std::shared_ptr<BodyPartDef>(new BodyPartDef());

                newBodyPartDef->ID=tree_item.second.get("ID","");
                newBodyPartDef->Name=tree_item.second.get("Name","");

                std::string materialID=tree_item.second.get("MaterialID","");
                newBodyPartDef->MaterialID=game::String2Enums::str2Material(materialID);

                std::string bodyProperties=tree_item.second.get("BodyProperties","");
                if (!bodyProperties.empty())
                {
                    std::vector<std::string> bodyPropertiesSplit = split(bodyProperties, ",");
                    for (auto bodyProperty : bodyPropertiesSplit)
                        newBodyPartDef->BodyProperties.push_back(game::String2Enums::str2BodyPartProperty(bodyProperty));
                }

                std::string bodyFunction=tree_item.second.get("BodyFunction","None");
                newBodyPartDef->BodyFunction = game::String2Enums::str2BodyFunction(bodyFunction);

                newBodyPartDef->Symmetrical = tree_item.second.get("Symmetrical",false);
                newBodyPartDef->ToHitWeight = tree_item.second.get("ToHitWeight",0.0f);
                newBodyPartDef->HarvestedItem = tree_item.second.get("HarvestedItem","");
                newBodyPartDef->HarvestedQuantity = tree_item.second.get("HarvestedQuantity",1);

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
                    std::shared_ptr<BodyDef> newBodyDef=std::shared_ptr<BodyDef>(new BodyDef());
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
                                int index = std::distance(body_sections_container.begin(), iter);
                                newBodyDef->MainBody=body_sections_container.at(index);
                            }
                        }
                    }

                    mBodyDefs.push_back(newBodyDef);
                    body_sections_container.clear();
                }
                else
                    {
                    std::shared_ptr<BodySectionDef> newBodySectionDef = std::shared_ptr<BodySectionDef>(
                            new BodySectionDef());

                    newBodySectionDef->ID = tree_item.second.get("ID", "");
                    newBodySectionDef->Name = tree_item.second.get("Name", "");
                    newBodySectionDef->BodyPartID = tree_item.second.get("BodyPartID", "");

                    std::string materialID = tree_item.second.get("MaterialID", "");
                    newBodySectionDef->MaterialID = game::String2Enums::str2Material(materialID);

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

                                if (iter != body_sections_container.end()) {
                                    int index = std::distance(body_sections_container.begin(), iter);
                                    newBodySectionDef->ConnectedSections.push_back(body_sections_container.at(index));
                                }
                            }
                        }
                    }

                    body_sections_container.push_back(newBodySectionDef);
                }
            }
        }
    }
}