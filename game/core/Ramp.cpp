//
// Created by tatiana on 28.03.2018.
//

#include "Ramp.h"
#include "game/map/MapCell.h"
#include "game/core/defines.h"
#include "game/properties/MaterialDef.h"
#include "game/properties/TerrainSprites.h"
#include "game/properties/TerrainSettings.h"

namespace game
{
    Ramp::Ramp(const map::vector3& position)
            :Construction(position)
    {

    }

    Ramp::~Ramp()
    {

    }

    std::shared_ptr<Ramp> Ramp::create(const map::vector3& position, std::string constructionID, const int& materialIndex,
            ConstructOrientation orientation)
    {
        std::shared_ptr<Ramp> ptr=std::shared_ptr<Ramp>(new Ramp(position));
        ptr->init(constructionID, materialIndex, orientation);
        return ptr;
    }

    void Ramp::init(std::string constructionID, const int& materialIndex, ConstructOrientation orientation)
    {
        mMaterialIndex=materialIndex;
        Construction::init(constructionID,orientation);
    }

    void Ramp::spawn(std::shared_ptr<map::MapCell> mapCell)
    {
        mapCell->setRamp(shared_from_this());
    }

    int Ramp::materialIndex() const
    {
        return mMaterialIndex;
    }

    std::vector<draw_info> Ramp::get_draw_info(const int& x, const int& y, const int& z,
            const view_orientation& orientation) const
    {
        std::shared_ptr<const properties::MaterialDef> materialDef=GAME_DEFINITIONS->materialDefinition(mMaterialIndex);
        const gui::ZColor color=materialDef->Color;
        const std::string materialID=materialDef->ID;
        std::shared_ptr<const properties::TerrainSprites> terrainSprite=TERRAIN_SETTINGS->terrainSprite(materialID);

        std::shared_ptr<const map::MapCell> cell1,cell2,cell3,cell4;
        if (orientation==view_orientation::front_left)
        {
            cell1=WORLD_MAP->cell(x-1,y,z);
            cell2=WORLD_MAP->cell(x,y+1,z);
            cell3=WORLD_MAP->cell(x+1,y,z);
            cell4=WORLD_MAP->cell(x,y-1,z);
        }
        else if (orientation==view_orientation::front_right)
        {
            cell1=WORLD_MAP->cell(y,x-1,z);
            cell2=WORLD_MAP->cell(y-1,x,z);
            cell3=WORLD_MAP->cell(y,x+1,z);
            cell4=WORLD_MAP->cell(y+1,x,z);
        }
        else if (orientation==view_orientation::back_right)
        {
            cell1=WORLD_MAP->cell(x+1,y,z);
            cell2=WORLD_MAP->cell(x,y-1,z);
            cell3=WORLD_MAP->cell(x-1,y,z);
            cell4=WORLD_MAP->cell(x,y+1,z);
        }
        else if (orientation==view_orientation::back_left)
        {
            cell1=WORLD_MAP->cell(y,x+1,z);
            cell2=WORLD_MAP->cell(y+1,x,z);
            cell3=WORLD_MAP->cell(y,x-1,z);
            cell4=WORLD_MAP->cell(y-1,x,z);
        }

        bool hasSupport1,hasSupport2,hasSupport3,hasSupport4;

        if (cell1 == nullptr)
            hasSupport1=false;
        else
            hasSupport1=cell1->hasWall();

        if (cell2 == nullptr)
            hasSupport2=false;
        else
            hasSupport2=cell2->hasWall();

        if (cell3 == nullptr)
            hasSupport3=false;
        else
            hasSupport3=cell3->hasWall();

        if (cell4 == nullptr)
            hasSupport4=false;
        else
            hasSupport4=cell4->hasWall();

        std::string spriteID;
        if (hasSupport1 && hasSupport2)
            spriteID="";
        else if (hasSupport1 && hasSupport3)
            spriteID="";
        else if (hasSupport2 && hasSupport4)
            spriteID="";
        else if (hasSupport3 && hasSupport4)
            spriteID="";
        else if (hasSupport1 && hasSupport4)
            spriteID="";
        else if (hasSupport2 && hasSupport3)
            spriteID="";
        else if (hasSupport1)
            spriteID=terrainSprite->TopRightRamp;
        else if (hasSupport2)
            spriteID=terrainSprite->BottomRightRamp;
        else if (hasSupport3)
            spriteID=terrainSprite->BottomLeftRamp;
        else if (hasSupport4)
            spriteID=terrainSprite->TopLeftRamp;

        std::vector<draw_info> result(1);

        draw_info item;
        item.color=color;
        item.spriteID=spriteID;
        item.order=1;
        result[0]=item;

        return result;
    }
}