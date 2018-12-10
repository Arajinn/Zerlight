//
// Created by tatiana on 29.11.18.
//

#ifndef ZERLIGHT_MAPWIDGET_H
#define ZERLIGHT_MAPWIDGET_H

#include "guichan/gui/ZContainer.h"
#include "guichan/gui/MouseListener.h"
#include "guichan/gui/KeyListener.h"
#include "game/core/enums.h"

namespace gui
{
    class ZImage;
}

class MapWidget : public gui::ZContainer, public gui::MouseListener, public gui::KeyListener{
public:
    MapWidget();
    ~MapWidget();

    virtual void init();
    virtual void shutdown();

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
                              const int& xPlot, const int& yPlot, const int& imageSize, const gui::ZColor& color);
};


#endif //ZERLIGHT_MAPWIDGET_H
