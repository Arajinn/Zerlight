//
// Created by tatiana on 29.11.18.
//

#include "MapWidget.h"
#include "guichan/gui/Graphics.h"
#include "guichan/gui/KeyEvent.h"
#include "guichan/gui/MouseEvent.h"
#include "game/core/defines.h"
#include "game/core/Ramp.h"
#include "game/map/Map.h"
#include "game/map/MapCell.h"
#include "game/properties/MaterialDef.h"
#include "game/properties/TerrainSettings.h"
#include "game/properties/TerrainSprites.h"
#include "game/properties/SpriteDef.h"

#include <SDL2/SDL_timer.h>

const int default_image_size=32;
const int zoom_min=1;
const int zoom_max=6;

MapWidget::MapWidget()
        :gui::ZContainer()
        ,gui::MouseListener()
        ,gui::KeyListener()
{

}

MapWidget::~MapWidget()
{

}

void MapWidget::init()
{
    ZContainer::init();

    addMouseListener(gui::MouseListener::downcasted_shared_from_this<MapWidget>());
    addKeyListener(gui::KeyListener::downcasted_shared_from_this<MapWidget>());

    mMouseLeftButtonPressedXPos=-1;
    mMouseLeftButtonPressedYPos=-1;
    mIsCtrlButtonPressed=false;
    mIsMouseLeftButtonPressed=false;

    mZLevel=0;

    mZoomCoeff=1;
    mDeltaX=0;
    mDeltaY=0;
    //xStart=getWidth()-150;
    //yStart=100;
    xStart=500;
    yStart=300;

    dx_wall=16.0f/32.0f;
    dy_wall_small=8.0f/32.0f;
    dy_wall_large=16.0f/32.0f;
    dy_floor_small=8.0f/32.0f;
    dy_floor_large=4.0f/32.0f;

    mOrientation=game::view_orientation::front_left;
}

void MapWidget::shutdown()
{
    gui::ZContainer::shutdown();
}

void MapWidget::keyPressed(gui::KeyEvent& keyEvent)
{
    gui::Key key = keyEvent.getKey();
    auto key_value=key.getValue();

    switch (key_value)
    {
        case gui::Key::LeftControl:
        case gui::Key::RightControl:
            mIsCtrlButtonPressed = true;
            std::cout << "map widget ctrl button pressed" << std::endl;
            keyEvent.consume();
            break;
        default:
            break;
    }
}

void MapWidget::keyReleased(gui::KeyEvent& keyEvent)
{
    gui::Key key = keyEvent.getKey();
    auto key_value=key.getValue();

    switch (key_value)
    {
        case gui::Key::LeftControl:
        case gui::Key::RightControl:
            mIsCtrlButtonPressed = false;
            std::cout << "map widget ctrl button released" << std::endl;
            keyEvent.consume();
            break;
        default:
            break;
    }
}

void MapWidget::mouseClicked(gui::MouseEvent& mouseEvent)
{
    std::cout << "map widget mouse clicked" << std::endl;
    const float clickedTime=SDL_GetTicks();
    const float deltaTime=clickedTime-mMousePressTime;
    if (deltaTime<300)
        changeViewDirectionToNext();
}

void MapWidget::mouseWheelMovedUp(gui::MouseEvent& mouseEvent)
{
    if (mIsCtrlButtonPressed)
    {
        mZoomCoeff+=1;
        zoomChanged();
    }
    else
    {
        mZLevel++;
        zLevelChanged();
    }
}

void MapWidget::mouseWheelMovedDown(gui::MouseEvent& mouseEvent)
{
    if (mIsCtrlButtonPressed)
    {
        mZoomCoeff-=1;
        zoomChanged();
    }
    else
    {
        mZLevel--;
        zLevelChanged();
    }
}

void MapWidget::mousePressed(gui::MouseEvent& mouseEvent)
{
    if (mouseEvent.isConsumed())
    {
        return;
    }

    if (mouseEvent.getButton() == gui::MouseEvent::Left)
    {
        mIsMouseLeftButtonPressed = true;

        mMouseLeftButtonPressedXPos=mouseEvent.getX();
        mMouseLeftButtonPressedYPos=mouseEvent.getY();

        mDeltaX=0;
        mDeltaY=0;

        mMousePressTime = SDL_GetTicks();

        mouseEvent.consume();
    }
}

void MapWidget::mouseReleased(gui::MouseEvent& mouseEvent)
{
    std::cout << "map widget mouse released" << std::endl;

    if (mouseEvent.getButton() == gui::MouseEvent::Left && mIsMouseLeftButtonPressed)
    {
        mIsMouseLeftButtonPressed = false;
        distributeActionEvent();
        mouseEvent.consume();

        xStart-=mDeltaX;
        yStart-=mDeltaY;
        mDeltaX=0;
        mDeltaY=0;
    }
    else if (mouseEvent.getButton() == gui::MouseEvent::Left)
    {
        mIsMouseLeftButtonPressed = false;
        mouseEvent.consume();
    }
}

void MapWidget::mouseMoved(gui::MouseEvent& mouseEvent)
{

}

void MapWidget::mouseDragged(gui::MouseEvent& mouseEvent)
{
    if (mIsMouseLeftButtonPressed)
    {
        auto newXPos=mouseEvent.getX();
        auto newYPos=mouseEvent.getY();

        mDeltaX=mMouseLeftButtonPressedXPos-newXPos;
        mDeltaY=mMouseLeftButtonPressedYPos-newYPos;
    }
}

void MapWidget::changeViewDirectionToNext()
{
    switch (mOrientation)
    {
        case game::view_orientation::front_left:
            mOrientation=game::view_orientation::front_right;
            break;
        case game::view_orientation::front_right:
            mOrientation=game::view_orientation::back_right;
            break;
        case game::view_orientation::back_right:
            mOrientation=game::view_orientation::back_left;
            break;
        case game::view_orientation::back_left:
            mOrientation=game::view_orientation::front_left;
            break;
    }
}

void MapWidget::zoomChanged()
{
    if (mZoomCoeff>zoom_max)
        mZoomCoeff=zoom_max;

    if (mZoomCoeff<zoom_min)
        mZoomCoeff=zoom_min;
}

void MapWidget::zLevelChanged()
{
//    if (mZLevel<mZMin)
//        mZLevel=mZMin;
//
//    if (mZLevel>mZMax)
//        mZLevel=mZMax;

    //std::cout << "z level: " << mZLevel << std::endl;
}

bool MapWidget::isPixelInWindow(const int& x, const int& y) const
{
    bool checkX1=(x>=-50);
    bool checkX2=(x<(getWidth()+50));
    bool checkY1=(y>=-50);
    bool checkY2=(y<(getHeight()+50));
    bool checkX=checkX1*checkX2;
    bool checkY=checkY1*checkY2;
    return checkX*checkY;
}

void MapWidget::drawImage(std::shared_ptr<gui::Graphics> graphics, std::shared_ptr<const gui::ZImage> image,
                             const int& xPlot, const int& yPlot, const int& imageSize, const gui::ZColor& color)
{
    graphics->drawImageScale(image, xPlot, yPlot, xPlot+imageSize, yPlot+imageSize, imageSize, imageSize, color);
}

void MapWidget::draw_item(std::shared_ptr<gui::Graphics> graphics)
{
    ZContainer::draw_item(graphics);

    graphics->textureMixOff();

    int iSize=WORLD_MAP->mapWidth();
    int jSize=WORLD_MAP->mapHeight();
    int kSize=WORLD_MAP->mapDepth();

    int imageSize=mZoomCoeff*default_image_size;

    int x0=xStart-mDeltaX;
    int y0=yStart-mDeltaY;

    int dxWall = int(float(imageSize) * dx_wall);
    int dyWall = int(float(imageSize) * dy_wall_small);

    int dxFloor = int(float(imageSize) * dx_wall);
    int dyFloor = int(float(imageSize) * dy_floor_small);

    int zIndex=0;
    int dzIndex=1;
    int xIndex,dxIndex;
    int yIndex,dyIndex;
    for (int plotZIndex=0;plotZIndex<kSize;plotZIndex++,zIndex=zIndex+dzIndex)
    {
        if (mOrientation==game::view_orientation::front_left)
        {
            xIndex = 0;
            dxIndex = 1;
        }
        else if (mOrientation==game::view_orientation::front_right)
        {
            xIndex = 0;
            dxIndex = 1;
        }
        else if (mOrientation==game::view_orientation::back_right)
        {
            xIndex = iSize-1;
            dxIndex = -1;
        }
        else if (mOrientation==game::view_orientation::back_left)
        {
            xIndex = iSize-1;
            dxIndex = -1;
        }

        for (int plotXIndex = 0; plotXIndex < iSize; plotXIndex++,xIndex=xIndex+dxIndex)
        {
            int xFloorPlot = x0 - int(dx_wall * float(plotXIndex) * float(imageSize));
            int yFloorPlot = y0 + int(dy_wall_small * float(plotXIndex) * float(imageSize))
                             - int(dy_wall_large*float(plotZIndex)*float(imageSize))
                             - int(dy_floor_large*float(plotZIndex)*float(imageSize));

            int xWallPlot = x0 - int(dx_wall * float(plotXIndex) * float(imageSize));
            int yWallPlot = y0 + int(dy_wall_small * float(plotXIndex) * float(imageSize))
                            - int(dy_wall_large*float(plotZIndex)*float(imageSize))
                            - int(dy_floor_large*float(plotZIndex+1)*float(imageSize));

            if (mOrientation==game::view_orientation::front_left)
            {
                yIndex = 0;
                dyIndex = 1;
            }
            else if (mOrientation==game::view_orientation::front_right)
            {
                yIndex = jSize-1;
                dyIndex = -1;
            }
            else if (mOrientation==game::view_orientation::back_right)
            {
                yIndex = jSize-1;
                dyIndex = -1;
            }
            else if (mOrientation==game::view_orientation::back_left)
            {
                yIndex = 0;
                dyIndex = 1;
            }

            for (int plotYIndex = 0; plotYIndex < jSize; plotYIndex++,yIndex=yIndex+dyIndex,
                    xWallPlot += dxWall,yWallPlot += dyWall,xFloorPlot += dxFloor,yFloorPlot += dyFloor)
            {
                if ((isPixelInWindow(xWallPlot,yWallPlot)==false)
                    && (isPixelInWindow(xWallPlot+imageSize,yWallPlot+imageSize)==false))
                    continue;

                std::shared_ptr<const map::MapCell> cell=nullptr;
                if (mOrientation==game::view_orientation::front_left)
                    cell=WORLD_MAP->cell(xIndex,yIndex,zIndex);
                else if (mOrientation==game::view_orientation::front_right)
                    cell=WORLD_MAP->cell(yIndex,xIndex,zIndex);
                else if (mOrientation==game::view_orientation::back_right)
                    cell=WORLD_MAP->cell(xIndex,yIndex,zIndex);
                else if (mOrientation==game::view_orientation::back_left)
                    cell=WORLD_MAP->cell(yIndex,xIndex,zIndex);

                //if (!cell->isOutside())
                //    continue;

                bool cell_visibility=cell->isVisible();
                if (!cell_visibility)
                {
                    bool xIsBond=((xIndex==0) || (xIndex==iSize-1));
                    bool yIsBond=((yIndex==0) || (yIndex==jSize-1));
                    if (xIsBond || yIsBond)
                        cell_visibility=true;
                }

                if (cell_visibility)
                {
                    if (cell->hasNaturalFloor())
                    {
                        const int floorMaterialIndex=cell->naturalFloor();
                        std::shared_ptr<const properties::MaterialDef> materialDef=GAME_DEFINITIONS->materialDefinition(floorMaterialIndex);
                        const gui::ZColor color=materialDef->Color;
                        const std::string materialID=materialDef->ID;
                        const std::string spriteID=TERRAIN_SETTINGS->terrainSprite(materialID)->FloorID;
                        std::shared_ptr<const properties::SpriteDef> spriteDef=GAME_DEFINITIONS->spriteDefinition(spriteID);
                        drawImage(graphics, spriteDef->Image, xFloorPlot, yFloorPlot, imageSize, color);
                    }

                    if (cell->hasNaturalWall())
                    {
                        const int wallMaterialIndex=cell->naturalWall();
                        std::shared_ptr<const properties::MaterialDef> materialDef=GAME_DEFINITIONS->materialDefinition(wallMaterialIndex);
                        const gui::ZColor color=materialDef->Color;
                        const std::string materialID=materialDef->ID;
                        const std::string spriteID=TERRAIN_SETTINGS->terrainSprite(materialID)->WallID;
                        std::shared_ptr<const properties::SpriteDef> spriteDef=GAME_DEFINITIONS->spriteDefinition(spriteID);
                        drawImage(graphics, spriteDef->Image, xWallPlot, yWallPlot, imageSize, color);
                    }

                    if (cell->hasRamp())
                    {
                        std::shared_ptr<const game::Ramp> ramp=cell->ramp();
                        std::vector<properties::TileDef> draw_infos=ramp->get_draw_info(xIndex,yIndex,zIndex,mOrientation);
                        properties::TileDef info=draw_infos.at(0);

                        if (!info.SpriteID.empty())
                        {
                            std::shared_ptr<const properties::SpriteDef> spriteDef=GAME_DEFINITIONS->spriteDefinition(info.SpriteID);
                            drawImage(graphics, spriteDef->Image, xWallPlot, yWallPlot, imageSize, info.Color);
                        }
                    }

                    if (cell->getObjectsCount()>0)
                    {
                        std::vector<std::shared_ptr<game::GameEntity>> objects=cell->getObjects();

                        for (auto object : objects)
                        {
                            std::vector<properties::TileDef> draw_infos=object->get_draw_info(xIndex,yIndex,zIndex,mOrientation);
                            for (const auto& draw_info : draw_infos)
                            {
                                if (!draw_info.SpriteID.empty())
                                {
                                    std::shared_ptr<const properties::SpriteDef> spriteDef=GAME_DEFINITIONS->spriteDefinition(draw_info.SpriteID);
                                    drawImage(graphics, spriteDef->Image, xWallPlot, yWallPlot, imageSize, draw_info.Color);
                                }
                            }

                            if (draw_infos.size()==0)
                            {
                                int t=0;
                            }
                        }
                    }
                }
            }
        }
    }

    graphics->textureMixOn();
}