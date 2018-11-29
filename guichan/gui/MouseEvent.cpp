//
// Created by tatiana on 27.09.18.
//

#include "MouseEvent.h"

namespace gui
{
    MouseEvent::MouseEvent(std::shared_ptr<ZWidget>  source,
                           std::shared_ptr<ZWidget> distributor,
                           bool isShiftPressed,
                           bool isControlPressed,
                           bool isAltPressed,
                           bool isMetaPressed,
                           unsigned int type,
                           unsigned int button,
                           int x,
                           int y,
                           int clickCount)
            :InputEvent(source,
                        distributor,
                        isShiftPressed,
                        isControlPressed,
                        isAltPressed,
                        isMetaPressed),
             mType(type),
             mButton(button),
             mX(x),
             mY(y),
             mClickCount(clickCount)
    {

    }

    unsigned int MouseEvent::getButton() const
    {
        return mButton;
    }

    int MouseEvent::getX() const
    {
        return mX;
    }

    int MouseEvent::getY() const
    {
        return mY;
    }

    int MouseEvent::getClickCount() const
    {
        return mClickCount;
    }

    unsigned int MouseEvent::getType() const
    {
        return mType;
    }

    void MouseEvent::setX(const int& x)
    {
        mX=x;
    }

    void MouseEvent::setY(const int& y)
    {
        mY=y;
    }
}