//
// Created by tatiana on 29.11.18.
//

#ifndef ZERLIGHT_MAPWIDGET_H
#define ZERLIGHT_MAPWIDGET_H

#include "guichan/gui/ZContainer.h"
#include "guichan/gui/MouseListener.h"
#include "guichan/gui/KeyListener.h"
#include "guichan/gui/ActionListener.h"
#include "game/core/enums.h"

namespace gui
{
    class ZImage;
    class ZButton;
    class ImageFont;
}

namespace interface
{
    class MainMenuWidget;
    class NewGameWidget;
    class CreateWorldLoadWidget;
    class HUDWidget;
}

class MapWidget : public gui::ZContainer, public gui::MouseListener, public gui::KeyListener, public gui::ActionListener{
public:
    MapWidget();
    ~MapWidget();

    virtual void init();
    virtual void shutdown();

    void action(const gui::ActionEvent& actionEvent);

    void resize();

    virtual void draw_item(std::shared_ptr<gui::Graphics> graphics) override;

    // Inherited from KeyListener
    virtual void keyPressed(gui::KeyEvent& keyEvent);
    virtual void keyReleased(gui::KeyEvent& keyEvent);

    // Inherited from MouseListener
    virtual void mouseClicked(gui::MouseEvent& mouseEvent);
    virtual void mouseWheelMovedUp(gui::MouseEvent& mouseEvent);
    virtual void mouseWheelMovedDown(gui::MouseEvent& mouseEvent);
    virtual void mousePressed(gui::MouseEvent& mouseEvent);
    virtual void mouseReleased(gui::MouseEvent& mouseEvent);
    virtual void mouseMoved(gui::MouseEvent& mouseEvent);
    virtual void mouseDragged(gui::MouseEvent& mouseEvent);

private:
    enum class DisplayMode
    {
        MainMenuWidget,
        NewGameWidget,
        CreateWorldLoadWidget,
        MapWidget
    };

    DisplayMode mDisplayMode;
    void changeDisplayMode();

    int mMouseLeftButtonPressedXPos,mMouseLeftButtonPressedYPos;
    bool mIsCtrlButtonPressed;
    bool mIsMouseLeftButtonPressed;
    float mMousePressTime;

    void changeViewDirectionToNext();

    int mZLevel;
    void zLevelChanged();

    int mZoomCoeff;
    void zoomChanged();

    int mDeltaX, mDeltaY;
    int xStart,yStart;

    float dx_wall, dy_wall_small, dy_wall_large, dy_floor_small, dy_floor_large;

    game::view_orientation mOrientation;

    bool isPixelInWindow(const int& x, const int& y) const;

    void drawImage(std::shared_ptr<gui::Graphics> graphics, std::shared_ptr<const gui::ZImage> image,
                              const int& xPlot, const int& yPlot, const int& imageSize, const gui::ZColor& color, bool xFlip=false);

    void initFonts();
    void clearFonts();
    std::shared_ptr<gui::ImageFont> mFont;
    std::shared_ptr<gui::ImageFont> mSmallBlackFont;
    std::shared_ptr<gui::ImageFont> mWhiteFont;
    std::shared_ptr<gui::ImageFont> mWhiteFontSmall;
    std::shared_ptr<gui::ImageFont> mHighLightFont;

    void initMainMenuWidget();
    void resizeMainMenuWidget();
    void clearMainMenuWidget();
    std::shared_ptr<interface::MainMenuWidget> mMainMenuWidget;

    void initNewGameWidget();
    void resizeNewGameWidget();
    void clearNewGameWidget();
    std::shared_ptr<interface::NewGameWidget> mNewGameWidget;

    void initCreateWorldLoadWidget();
    void resizeCreateWorldLoadWidget();
    void clearCreateWorldLoadWidget();
    std::shared_ptr<interface::CreateWorldLoadWidget> mCreateWorldLoadWidget;

    void initHUDWidget();
    void resizeHUDWidget();
    void clearHUDWidget();
    std::shared_ptr<interface::HUDWidget> mHUDWidget;
};


#endif //ZERLIGHT_MAPWIDGET_H
