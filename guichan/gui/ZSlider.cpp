//
// Created by tatiana on 28.09.18.
//

#include "ZSlider.h"
#include "Graphics.h"
#include "MouseEvent.h"
#include "KeyEvent.h"

namespace gui
{
    ZSlider::ZSlider()
    {

    }

    ZSlider::~ZSlider()
    {

    }

    void ZSlider::init(double scaleEnd)
    {
        ZWidget::init();

        mDragged = false;

        mScaleStart = 0;
        mScaleEnd = scaleEnd;

        setFocusable(true);
        setFrameSize(1);
        setOrientation(Horizontal);
        setValue(0);
        setStepLength(scaleEnd / 10);
        setMarkerLength(10);

        addMouseListener(MouseListener::downcasted_shared_from_this<ZSlider>());
        addKeyListener(KeyListener::downcasted_shared_from_this<ZSlider>());
    }

    void ZSlider::init(double scaleStart, double scaleEnd)
    {
        ZWidget::init();

        mDragged = false;

        mScaleStart = scaleStart;
        mScaleEnd = scaleEnd;

        setFocusable(true);
        setFrameSize(1);
        setOrientation(Horizontal);
        setValue(scaleStart);
        setStepLength((scaleEnd  - scaleStart)/ 10);
        setMarkerLength(10);

        addMouseListener(MouseListener::downcasted_shared_from_this<ZSlider>());
        addKeyListener(KeyListener::downcasted_shared_from_this<ZSlider>());
    }

    void ZSlider::setScale(double scaleStart, double scaleEnd)
    {
        mScaleStart = scaleStart;
        mScaleEnd = scaleEnd;
    }

    double ZSlider::getScaleStart() const
    {
        return mScaleStart;
    }

    void ZSlider::setScaleStart(double scaleStart)
    {
        mScaleStart = scaleStart;
    }

    double ZSlider::getScaleEnd() const
    {
        return mScaleEnd;
    }

    void ZSlider::setScaleEnd(double scaleEnd)
    {
        mScaleEnd = scaleEnd;
    }

    void ZSlider::draw_item(std::shared_ptr<Graphics> graphics)
    {
        ZColor shadowColor = getBaseColor() - 0x101010;
        int alpha = getBaseColor().a;
        shadowColor.a = alpha;

        graphics->setColor(shadowColor);
        graphics->fillRectangle(0, 0, getWidth(), getHeight());

        drawMarker(graphics);
    }

    void ZSlider::drawMarker(std::shared_ptr<Graphics>graphics)
    {
        ZColor faceColor = getBaseColor();
        ZColor highlightColor, shadowColor;
        int alpha = getBaseColor().a;
        highlightColor = faceColor + 0x303030;
        highlightColor.a = alpha;
        shadowColor = faceColor - 0x303030;
        shadowColor.a = alpha;

        graphics->setColor(faceColor);

        if (getOrientation() == Horizontal)
        {
            int v = getMarkerPosition();
            graphics->fillRectangle(v + 1, 1, getMarkerLength() - 2, getHeight() - 2);
            graphics->setColor(highlightColor);
            graphics->drawLine(v, 0, v + getMarkerLength() - 1,0);
            graphics->drawLine(v, 0, v, getHeight() - 1);
            graphics->setColor(shadowColor);
            graphics->drawLine(v + getMarkerLength() - 1, 1, v + getMarkerLength() - 1, getHeight() - 1);
            graphics->drawLine(v + 1, getHeight() - 1, v + getMarkerLength() - 1, getHeight() - 1);

            if (isFocused())
            {
                graphics->setColor(getForegroundColor());
                graphics->drawRectangle(v + 2, 2, getMarkerLength() - 4, getHeight() - 4);
            }
        }
        else
        {
            int v = (getHeight() - getMarkerLength()) - getMarkerPosition();
            graphics->fillRectangle(1, v + 1, getWidth() - 2, getMarkerLength() - 2);
            graphics->setColor(highlightColor);
            graphics->drawLine(0, v, 0, v + getMarkerLength() - 1);
            graphics->drawLine(0, v, getWidth() - 1, v);
            graphics->setColor(shadowColor);
            graphics->drawLine(1, v + getMarkerLength() - 1, getWidth() - 1, v + getMarkerLength() - 1);
            graphics->drawLine(getWidth() - 1, v + 1, getWidth() - 1, v + getMarkerLength() - 1);

            if (isFocused())
            {
                graphics->setColor(getForegroundColor());
                graphics->drawRectangle(2, v + 2, getWidth() - 4, getMarkerLength() - 4);
            }
        }
    }

    void ZSlider::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::Left
            && mouseEvent.getX() >= 0
            && mouseEvent.getX() <= getWidth()
            && mouseEvent.getY() >= 0
            && mouseEvent.getY() <= getHeight())
        {
            if (getOrientation() == Horizontal)
            {
                setValue(markerPositionToValue(mouseEvent.getX() - getMarkerLength() / 2));
            }
            else
            {
                setValue(markerPositionToValue(getHeight() - mouseEvent.getY() - getMarkerLength() / 2));
            }

            distributeActionEvent();
        }
    }

    void ZSlider::mouseDragged(MouseEvent& mouseEvent)
    {
        if (getOrientation() == Horizontal)
        {
            setValue(markerPositionToValue(mouseEvent.getX() - getMarkerLength() / 2));
        }
        else
        {
            setValue(markerPositionToValue(getHeight() - mouseEvent.getY() - getMarkerLength() / 2));
        }

        distributeActionEvent();

        mouseEvent.consume();
    }

    void ZSlider::setValue(double value)
    {
        if (value > getScaleEnd())
        {
            mValue = getScaleEnd();
            return;
        }

        if (value < getScaleStart())
        {
            mValue = getScaleStart();
            return;
        }

        mValue = value;
    }

    double ZSlider::getValue() const
    {
        return mValue;
    }

    int ZSlider::getMarkerLength() const
    {
        return mMarkerLength;
    }

    void ZSlider::setMarkerLength(int length)
    {
        mMarkerLength = length;
    }

    void ZSlider::keyPressed(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();

        if (getOrientation() == Horizontal)
        {
            if (key.getValue() == Key::Right)
            {
                setValue(getValue() + getStepLength());
                distributeActionEvent();
                keyEvent.consume();
            }
            else if (key.getValue() == Key::Left)
            {
                setValue(getValue() - getStepLength());
                distributeActionEvent();
                keyEvent.consume();
            }
        }
        else
        {
            if (key.getValue() == Key::Up)
            {
                setValue(getValue() + getStepLength());
                distributeActionEvent();
                keyEvent.consume();
            }
            else if (key.getValue() == Key::Down)
            {
                setValue(getValue() - getStepLength());
                distributeActionEvent();
                keyEvent.consume();
            }
        }
    }

    void ZSlider::setOrientation(ZSlider::Orientation orientation)
    {
        mOrientation = orientation;
    }

    ZSlider::Orientation ZSlider::getOrientation() const
    {
        return mOrientation;
    }

    double ZSlider::markerPositionToValue(int v) const
    {
        int w;
        if (getOrientation() == Horizontal)
        {
            w = getWidth();
        }
        else
        {
            w = getHeight();
        }

        double pos = v / ((double)w - getMarkerLength());
        return (1.0 - pos) * getScaleStart() + pos * getScaleEnd();

    }

    int ZSlider::valueToMarkerPosition(double value) const
    {
        int v;
        if (getOrientation() == Horizontal)
        {
            v = getWidth();
        }
        else
        {
            v = getHeight();
        }

        int w =  (int)((v - getMarkerLength()) * (value  - getScaleStart()) / (getScaleEnd() - getScaleStart()));

        if (w < 0)
        {
            return 0;
        }

        if (w > v - getMarkerLength())
        {
            return v - getMarkerLength();
        }

        return w;
    }

    void ZSlider::setStepLength(double length)
    {
        mStepLength = length;
    }

    double ZSlider::getStepLength() const
    {
        return mStepLength;
    }

    int ZSlider::getMarkerPosition() const
    {
        return valueToMarkerPosition(getValue());
    }

    void ZSlider::mouseWheelMovedUp(MouseEvent& mouseEvent)
    {
        setValue(getValue() + getStepLength());
        distributeActionEvent();

        mouseEvent.consume();
    }

    void ZSlider::mouseWheelMovedDown(MouseEvent& mouseEvent)
    {
        setValue(getValue() - getStepLength());
        distributeActionEvent();

        mouseEvent.consume();
    }
}