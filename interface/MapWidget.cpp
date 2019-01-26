//
// Created by tatiana on 29.11.18.
//

#include "MapWidget.h"
#include "InterfaceWidgetFactory.h"
#include "ActionConstants.h"
#include "MainMenuWidget.h"
#include "NewGameWidget.h"
#include "CreateWorldLoadWidget.h"
#include "guichan/gui/Graphics.h"
#include "guichan/gui/KeyEvent.h"
#include "guichan/gui/MouseEvent.h"
#include "guichan/gui/ImageFont.h"
#include "guichan/gui/ActionEvent.h"
#include "game/core/defines.h"
#include "game/core/Ramp.h"
#include "game/map/Map.h"
#include "game/map/MapCell.h"
#include "game/properties/MaterialDef.h"
#include "game/properties/TerrainSettings.h"
#include "game/properties/TerrainSprites.h"
#include "game/properties/SpriteDef.h"

#include <SDL2/SDL_timer.h>

#include <future>

const int default_image_size=32;
const int zoom_min=1;
const int zoom_max=6;

MapWidget::MapWidget()
        :gui::ZContainer()
        ,gui::MouseListener()
        ,gui::KeyListener()
        ,gui::ActionListener()
        ,mDisplayMode(DisplayMode::MainMenuWidget)
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

    initFonts();

    initMainMenuWidget();
    resizeMainMenuWidget();

    initNewGameWidget();
    resizeNewGameWidget();

    initCreateWorldLoadWidget();
    resizeCreateWorldLoadWidget();

    changeDisplayMode();
}

void MapWidget::shutdown()
{
    clearFonts();

    clearMainMenuWidget();
    clearNewGameWidget();
    clearCreateWorldLoadWidget();

    gui::ZContainer::shutdown();
}

void MapWidget::resize()
{
    resizeMainMenuWidget();
    resizeNewGameWidget();
    resizeCreateWorldLoadWidget();
}

void MapWidget::initFonts()
{
    mFont = std::make_shared<gui::ImageFont>("../images/interface/techyfontbig2.png",        " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");
    mHighLightFont = std::make_shared<gui::ImageFont>("../images/interface/techyfontbighighlight.png"," abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");
    mSmallBlackFont = std::make_shared<gui::ImageFont>("../images/interface/techyfontblack.png",       " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");
    mWhiteFont = std::make_shared<gui::ImageFont>("../images/interface/techyfontwhite.png",       " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");
    gui::ZWidget::setGlobalFont(mWhiteFont);
}

void MapWidget::clearFonts()
{
    mFont = nullptr;
    mHighLightFont = nullptr;
    mSmallBlackFont = nullptr;
    mWhiteFont = nullptr;
}

void MapWidget::initMainMenuWidget()
{
    mMainMenuWidget = InterfaceWidgetFactory::create_mainmenuwidget();
    mMainMenuWidget->setVisible(false);
    mMainMenuWidget->setOpaque(false);
    mMainMenuWidget->addActionListener(ActionListener::downcasted_shared_from_this<MapWidget>());
    add(mMainMenuWidget);
}

void MapWidget::resizeMainMenuWidget()
{
    mMainMenuWidget->setDimension(gui::Rectangle(0, 0, getWidth(), getHeight()));
    mMainMenuWidget->resize();
}

void MapWidget::clearMainMenuWidget()
{
    mMainMenuWidget=nullptr;
}

void MapWidget::initNewGameWidget()
{
    mNewGameWidget = InterfaceWidgetFactory::create_newgamewidget();
    mNewGameWidget->setVisible(false);
    mNewGameWidget->setOpaque(false);
    mNewGameWidget->addActionListener(ActionListener::downcasted_shared_from_this<MapWidget>());
    add(mNewGameWidget);
}

void MapWidget::resizeNewGameWidget()
{
    mNewGameWidget->setDimension(gui::Rectangle(0, 0, getWidth(), getHeight()));
    mNewGameWidget->resize();
}

void MapWidget::clearNewGameWidget()
{
    mNewGameWidget=nullptr;
}

void MapWidget::initCreateWorldLoadWidget()
{
    mCreateWorldLoadWidget = InterfaceWidgetFactory::create_createworldloadwidget();
    mCreateWorldLoadWidget->setVisible(false);
    mCreateWorldLoadWidget->setOpaque(false);
    add(mCreateWorldLoadWidget);
}

void MapWidget::resizeCreateWorldLoadWidget()
{
    mCreateWorldLoadWidget->setDimension(gui::Rectangle(0, 0, getWidth(), getHeight()));
    mCreateWorldLoadWidget->resize();
}

void MapWidget::clearCreateWorldLoadWidget()
{
    mNewGameWidget=nullptr;
}

void MapWidget::changeDisplayMode()
{
    if (mDisplayMode==DisplayMode::MainMenuWidget)
    {
        mMainMenuWidget->setVisible(true);
        mNewGameWidget->setVisible(false);
        mCreateWorldLoadWidget->setVisible(false);
    }
    else if (mDisplayMode==DisplayMode::NewGameWidget)
    {
        mMainMenuWidget->setVisible(false);
        mNewGameWidget->setVisible(true);
        mCreateWorldLoadWidget->setVisible(false);
    }
    else if (mDisplayMode==DisplayMode::CreateWorldLoadWidget)
    {
        mMainMenuWidget->setVisible(false);
        mNewGameWidget->setVisible(false);
        mCreateWorldLoadWidget->setVisible(true);
    }
    else if (mDisplayMode==DisplayMode::MapWidget)
    {
        mMainMenuWidget->setVisible(false);
        mNewGameWidget->setVisible(false);
        mCreateWorldLoadWidget->setVisible(false);
    }
}

void MapWidget::action(const gui::ActionEvent& actionEvent)
{
    const auto action_id=actionEvent.getId();
    if (action_id==action::NEW_GAME_ACTION)
    {
        mDisplayMode=DisplayMode::NewGameWidget;
        changeDisplayMode();
    }
    else if (action_id==action::BACK_TO_MAIN_WINDOW_ACTION)
    {
        mDisplayMode=DisplayMode::MainMenuWidget;
        changeDisplayMode();
    }
    else if (action_id==action::GENERATE_NEW_WORLD_ACTION)
    {
        GMINSTANCE->startGenerate();

        std::packaged_task<bool()> task(std::bind(&game::GameManager::generateNewWorld,GMINSTANCE.get()));
        std::future<bool> result = task.get_future();

        std::thread gameThread = std::thread(std::move(task));
        gameThread.detach();

        mDisplayMode=DisplayMode::CreateWorldLoadWidget;
        changeDisplayMode();
    }
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
    if (GMINSTANCE->currentMode()!=game::GameManager::CurrentMode::StandartWorld)
        return;

    std::cout << "map widget mouse clicked" << std::endl;
    const float clickedTime=SDL_GetTicks();
    const float deltaTime=clickedTime-mMousePressTime;
    if (deltaTime<300)
        changeViewDirectionToNext();
}

void MapWidget::mouseWheelMovedUp(gui::MouseEvent& mouseEvent)
{
    if (GMINSTANCE->currentMode()!=game::GameManager::CurrentMode::StandartWorld)
        return;

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
    if (GMINSTANCE->currentMode()!=game::GameManager::CurrentMode::StandartWorld)
        return;

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

    if (GMINSTANCE->currentMode()!=game::GameManager::CurrentMode::StandartWorld)
        return;

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
    if (GMINSTANCE->currentMode()!=game::GameManager::CurrentMode::StandartWorld)
        return;

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
    if (GMINSTANCE->currentMode()!=game::GameManager::CurrentMode::StandartWorld)
        return;

}

void MapWidget::mouseDragged(gui::MouseEvent& mouseEvent)
{
    if (GMINSTANCE->currentMode()!=game::GameManager::CurrentMode::StandartWorld)
        return;

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
                             const int& xPlot, const int& yPlot, const int& imageSize, const gui::ZColor& color, bool xFlip)
{
    graphics->drawImageScale(image, xPlot, yPlot, xPlot+imageSize, yPlot+imageSize, imageSize, imageSize, color, xFlip);
}

void MapWidget::draw_item(std::shared_ptr<gui::Graphics> graphics)
{
    //ZContainer::draw_item(graphics);

    if (GMINSTANCE->isGenerateFinish())
    {
        if (mDisplayMode==DisplayMode::CreateWorldLoadWidget)
        {
            mDisplayMode = DisplayMode::MapWidget;
            changeDisplayMode();
        }

        graphics->textureMixOff();

        const size_t iSize = WORLD_MAP->mapWidth();
        const size_t jSize = WORLD_MAP->mapHeight();
        const size_t kSize = WORLD_MAP->mapDepth();

        int imageSize = mZoomCoeff * default_image_size;

        int x0 = xStart - mDeltaX;
        int y0 = yStart - mDeltaY;

        const auto dxWall = int(float(imageSize) * dx_wall);
        const auto dyWall = int(float(imageSize) * dy_wall_small);

        const auto dxFloor = int(float(imageSize) * dx_wall);
        const auto dyFloor = int(float(imageSize) * dy_floor_small);

        int zIndex = 0;
        int dzIndex = 1;
        int xIndex, dxIndex;
        int yIndex, dyIndex;
        for (int plotZIndex = 0; plotZIndex < kSize; plotZIndex++, zIndex = zIndex + dzIndex) {
            if (mOrientation == game::view_orientation::front_left)
            {
                xIndex = 0;
                dxIndex = 1;
            }
            else if (mOrientation == game::view_orientation::front_right)
            {
                xIndex = 0;
                dxIndex = 1;
            }
            else if (mOrientation == game::view_orientation::back_right)
            {
                xIndex = (int)iSize - 1;
                dxIndex = -1;
            }
            else if (mOrientation == game::view_orientation::back_left)
            {
                xIndex = (int)iSize - 1;
                dxIndex = -1;
            }

            for (int plotXIndex = 0; plotXIndex < iSize; plotXIndex++, xIndex = xIndex + dxIndex) {
                int xFloorPlot = x0 - int(dx_wall * float(plotXIndex) * float(imageSize));
                int yFloorPlot = y0 + int(dy_wall_small * float(plotXIndex) * float(imageSize))
                                 - int(dy_wall_large * float(plotZIndex) * float(imageSize))
                                 - int(dy_floor_large * float(plotZIndex) * float(imageSize));

                int xWallPlot = x0 - int(dx_wall * float(plotXIndex) * float(imageSize));
                int yWallPlot = y0 + int(dy_wall_small * float(plotXIndex) * float(imageSize))
                                - int(dy_wall_large * float(plotZIndex) * float(imageSize))
                                - int(dy_floor_large * float(plotZIndex + 1) * float(imageSize));

                if (mOrientation == game::view_orientation::front_left)
                {
                    yIndex = 0;
                    dyIndex = 1;
                }
                else if (mOrientation == game::view_orientation::front_right)
                {
                    yIndex = (int)jSize - 1;
                    dyIndex = -1;
                }
                else if (mOrientation == game::view_orientation::back_right)
                {
                    yIndex = (int)jSize - 1;
                    dyIndex = -1;
                }
                else if (mOrientation == game::view_orientation::back_left)
                {
                    yIndex = 0;
                    dyIndex = 1;
                }

                for (int plotYIndex = 0; plotYIndex < jSize; plotYIndex++, yIndex = yIndex + dyIndex,
                                                                           xWallPlot += dxWall, yWallPlot += dyWall, xFloorPlot += dxFloor, yFloorPlot += dyFloor) {
                    if ((!isPixelInWindow(xWallPlot, yWallPlot))
                        && (!isPixelInWindow(xWallPlot + imageSize, yWallPlot + imageSize)))
                        continue;

                    std::shared_ptr<const map::MapCell> cell = nullptr;
                    if (mOrientation == game::view_orientation::front_left)
                        cell = WORLD_MAP->cell(xIndex, yIndex, zIndex);
                    else if (mOrientation == game::view_orientation::front_right)
                        cell = WORLD_MAP->cell(yIndex, xIndex, zIndex);
                    else if (mOrientation == game::view_orientation::back_right)
                        cell = WORLD_MAP->cell(xIndex, yIndex, zIndex);
                    else if (mOrientation == game::view_orientation::back_left)
                        cell = WORLD_MAP->cell(yIndex, xIndex, zIndex);

                    //if (!cell->isOutside())
                    //    continue;

                    bool cell_visibility = cell->isVisible();
                    //bool cell_visibility=true;
                    if (!cell_visibility)
                    {
                        bool xIsBond = ((xIndex == 0) || (xIndex == iSize - 1));
                        bool yIsBond = ((yIndex == 0) || (yIndex == jSize - 1));
                        if (xIsBond || yIsBond)
                            cell_visibility = true;
                    }

                    if (cell_visibility) {
                        if (cell->hasNaturalFloor()) {
                            const int floorMaterialIndex = cell->naturalFloor();
                            std::shared_ptr<const properties::MaterialDef> materialDef = GAME_DEFINITIONS->materialDefinition(
                                    floorMaterialIndex);
                            const gui::ZColor color = materialDef->Color;
                            const game::MaterialID_t materialID = materialDef->ID;
                            const std::string spriteID = TERRAIN_SETTINGS->terrainSprite(materialID)->FloorID;
                            std::shared_ptr<const properties::SpriteDef> spriteDef = GAME_DEFINITIONS->spriteDefinition(
                                    spriteID);
                            drawImage(graphics, spriteDef->Image, xFloorPlot, yFloorPlot, imageSize, color);
                        }

                        if (cell->hasNaturalWall())
                        {
                            const int wallMaterialIndex = cell->naturalWall();
                            std::shared_ptr<const properties::MaterialDef> materialDef = GAME_DEFINITIONS->materialDefinition(
                                    wallMaterialIndex);
                            const gui::ZColor color = materialDef->Color;
                            const game::MaterialID_t materialID = materialDef->ID;
                            const std::string spriteID = TERRAIN_SETTINGS->terrainSprite(materialID)->WallID;
                            std::shared_ptr<const properties::SpriteDef> spriteDef = GAME_DEFINITIONS->spriteDefinition(
                                    spriteID);
                            drawImage(graphics, spriteDef->Image, xWallPlot, yWallPlot, imageSize, color);
                        }

                        if (cell->hasRamp())
                        {
                            std::shared_ptr<const game::Ramp> ramp = cell->ramp();
                            std::vector<properties::TileDef> draw_infos = ramp->get_draw_info(xIndex, yIndex, zIndex, mOrientation);
                            properties::TileDef info = draw_infos.at(0);

                            if (!info.SpriteID.empty())
                            {
                                std::shared_ptr<const properties::SpriteDef> spriteDef = GAME_DEFINITIONS->spriteDefinition( info.SpriteID);
                                drawImage(graphics, spriteDef->Image, xWallPlot, yWallPlot, imageSize, info.Color);
                            }
                        }

                        if (cell->getObjectsCount() > 0)
                        {
                            auto objects = cell->getObjects();

                            for (auto object : objects)
                            {
                                std::vector<properties::TileDef> draw_infos = object->get_draw_info(xIndex, yIndex, zIndex, mOrientation);
                                for (const auto &draw_info : draw_infos)
                                {
                                    if (!draw_info.SpriteID.empty())
                                    {
                                        std::shared_ptr<const properties::SpriteDef> spriteDef = GAME_DEFINITIONS->spriteDefinition( draw_info.SpriteID);
                                        drawImage(graphics, spriteDef->Image, xWallPlot, yWallPlot, imageSize, draw_info.Color);
                                    }
                                }

                                if (draw_infos.empty())
                                {
                                    int t = 0;
                                }
                            }
                        }

                        if (cell->getCharactersCount() > 0)
                        {
                            auto objects = cell->getCharacters();

                            for (auto object : objects)
                            {
                                std::vector<properties::TileDef> draw_infos = object->get_draw_info(xIndex, yIndex, zIndex, mOrientation);

                                auto delta_pos = object->delta_position();

                                //std::cout << "pos: " << delta_pos.x() << " " << delta_pos.y() << std::endl;

                                bool xFlip = false;
                                if (mOrientation == game::view_orientation::front_left)
                                {
                                    if (delta_pos.y() == -1)
                                        xFlip = true;
                                }
                                else if (mOrientation == game::view_orientation::front_right)
                                {
                                    if (delta_pos.x() == 1)
                                        xFlip = true;
                                }
                                else if (mOrientation == game::view_orientation::back_right)
                                {
                                    if (delta_pos.y() == 1)
                                        xFlip = true;
                                }
                                else if (mOrientation == game::view_orientation::back_left)
                                {
                                    if (delta_pos.x() == -1)
                                        xFlip = true;
                                }

                                for (const auto &draw_info : draw_infos)
                                {
                                    if (!draw_info.SpriteID.empty())
                                    {
                                        std::shared_ptr<const properties::SpriteDef> spriteDef = GAME_DEFINITIONS->spriteDefinition(
                                                draw_info.SpriteID);
                                        drawImage(graphics, spriteDef->Image, xWallPlot, yWallPlot, imageSize,
                                                  draw_info.Color, xFlip);
                                    }
                                }

                                if (draw_infos.empty())
                                {
                                    int t = 0;
                                }
                            }
                        }
                    }
                }
            }
        }

        graphics->textureMixOn();
    }
}