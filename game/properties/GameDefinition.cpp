//
// Created by tatiana.polozova on 27.03.2018.
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
#include <algorithm>

namespace properties {
    GameDefinition::GameDefinition()
    {
        mTerrainSettings=std::shared_ptr<TerrainSettings>(new TerrainSettings);
        mTerrainSettings->AirMaterialID="Air";

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
        mRaceDefinitions.push_back(settlerDefinition);

        std::shared_ptr<MaterialDef> airDef=std::shared_ptr<MaterialDef>(new MaterialDef());
        airDef->ID="Air";
        airDef->Name="air";
        airDef->Type=game::MaterialType::Air;
        airDef->Value=0.0f;
        airDef->Strength=0.0f;
        mMaterials.push_back(airDef);

        std::shared_ptr<MaterialDef> graniteDef=std::shared_ptr<MaterialDef>(new MaterialDef());
        graniteDef->ID="Granite";
        graniteDef->Name="granite";
        graniteDef->Type=game::MaterialType::Stone;
        graniteDef->Value=1.0f;
        graniteDef->Strength=0.7f;
        graniteDef->Color=gui::ZColor(128,128,128,255);
        mMaterials.push_back(graniteDef);

        std::shared_ptr<ConstructionDef> soilStairs=std::shared_ptr<ConstructionDef>(new ConstructionDef());
        soilStairs->ID="SoilStairs";
        soilStairs->Name="stairs";
        soilStairs->Prefix="soil";
        mConstructionDefs.push_back(soilStairs);

        std::shared_ptr<ItemDefinition> wineDefinition=std::shared_ptr<ItemDefinition>(new ItemDefinition());
        wineDefinition->ID="Wine";
        wineDefinition->Name="wine";
        wineDefinition->addEffect(game::ItemEffectType::Drink,50.0f);
        mItemDefinitions.push_back(wineDefinition);

        std::shared_ptr<ItemDefinition> fruitDefinition=std::shared_ptr<ItemDefinition>(new ItemDefinition());
        fruitDefinition->ID="Fruit";
        fruitDefinition->Name="fruit";
        fruitDefinition->addEffect(game::ItemEffectType::Food,15.0f);
        mItemDefinitions.push_back(fruitDefinition);

        mGameSettings=std::shared_ptr<GameSettings>(new GameSettings());
        mGameSettings->NutritionWeight=50.0f;
        mGameSettings->DepthDistanceBias=1.0f;

        this->initBodyPartDefs();
        this->initBodyDefs();
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
        auto iter=std::find_if(std::begin(mMaterials),std::end(mMaterials),[&id](std::shared_ptr<MaterialDef> const& value)
        {
           return (id==value->ID);
        });

        if (iter==std::end(mMaterials))
            return -1;
        else
            return std::distance(mMaterials.begin(),iter);
    }

    const std::shared_ptr<RaceDefinition>& GameDefinition::raceDefinition(const std::string& id) const
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

    const std::shared_ptr<ConstructionDef>& GameDefinition::constructionDefinition(const std::string& id) const
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

    const std::shared_ptr<ItemDefinition>& GameDefinition::itemDefinition(const std::string& id) const
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

    const std::shared_ptr<GameSettings>& GameDefinition::gameSettings() const
    {
        return mGameSettings;
    }

    const std::shared_ptr<BodyPartDef>& GameDefinition::bodyPartDefinition(const std::string& id) const
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

    const std::shared_ptr<BodyDef>& GameDefinition::bodyDefinition(const std::string& id) const
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

    const std::shared_ptr<MaterialDef>& GameDefinition::materialDefinition(const std::string& id) const
    {
        auto iter=std::find_if(std::begin(mMaterials),std::end(mMaterials),[&id](std::shared_ptr<MaterialDef> const& value)
        {
            return (id==value->ID);
        });

        if (iter!=std::end(mMaterials))
        {
            int index=std::distance(mMaterials.begin(),iter);
            return mMaterials.at(index);
        }
        else
            return nullptr;
    }
}