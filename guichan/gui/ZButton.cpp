//
// Created by tatiana on 27.09.18.
//

#include "ZButton.h"
#include "Exception.h"
#include "MouseEvent.h"
#include "KeyEvent.h"

namespace gui
{
    ZButton::ZButton()
        :ZWidget()
        ,MouseListener()
        ,KeyListener()
        ,FocusListener()
    {

    }

    void ZButton::init()
    {
        ZWidget::init();

        mHasMouse=false;
        mKeyPressed=false;
        mMousePressed=false;
        mAlignment=Graphics::Center;
        mSpacing=4;

        setFocusable(true);
        adjustSize();
        setFrameSize(1);

        addMouseListener(MouseListener::downcasted_shared_from_this<ZButton>());
        addKeyListener(KeyListener::downcasted_shared_from_this<ZButton>());
        addFocusListener(FocusListener::downcasted_shared_from_this<ZButton>());
    }

    void ZButton::init(const std::string& caption)
    {
        ZWidget::init();

        mCaption=caption;

        mHasMouse=false;
        mKeyPressed=false;
        mMousePressed=false;
        mAlignment=Graphics::Center;
        mSpacing=4;

        setFocusable(true);
        adjustSize();
        setFrameSize(1);

        addMouseListener(MouseListener::downcasted_shared_from_this<ZButton>());
        addKeyListener(KeyListener::downcasted_shared_from_this<ZButton>());
        addFocusListener(FocusListener::downcasted_shared_from_this<ZButton>());
    }

    void ZButton::shutdown()
    {
        ZWidget::shutdown();
    }

    void ZButton::setCaption(const std::string& caption)
    {
        mCaption = caption;
    }

    const std::string& ZButton::getCaption() const
    {
        return mCaption;
    }

    void ZButton::setAlignment(Graphics::Alignment alignment)
    {
        mAlignment = alignment;
    }

    Graphics::Alignment ZButton::getAlignment() const
    {
        return mAlignment;
    }

    void ZButton::setSpacing(unsigned int spacing)
    {
        mSpacing = spacing;
    }

    unsigned int ZButton::getSpacing() const
    {
        return mSpacing;
    }

    void ZButton::draw_item(std::shared_ptr<Graphics> graphics)
    {
        ZColor faceColor = getBaseColor();
        ZColor highlightColor, shadowColor;
        int alpha = getBaseColor().a;

        if (isPressed())
        {
            faceColor = faceColor - 0x303030;
            faceColor.a = alpha;
            highlightColor = faceColor - 0x303030;
            highlightColor.a = alpha;
            shadowColor = faceColor + 0x303030;
            shadowColor.a = alpha;
        }
        else
        {
            highlightColor = faceColor + 0x303030;
            highlightColor.a = alpha;
            shadowColor = faceColor - 0x303030;
            shadowColor.a = alpha;
        }

        graphics->setColor(faceColor);
        graphics->fillRectangle(1, 1, getDimension().width-1, getHeight() - 1);

        graphics->setColor(highlightColor);
        graphics->drawLine(0, 0, getWidth() - 1, 0);
        graphics->drawLine(0, 1, 0, getHeight() - 1);

        graphics->setColor(shadowColor);
        graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);
        graphics->drawLine(1, getHeight() - 1, getWidth() - 1, getHeight() - 1);

        graphics->setColor(getForegroundColor());

        int textX;
        int textY = getHeight() / 2 - getFont()->getHeight() / 2;

        switch (getAlignment())
        {
            case Graphics::Left:
                textX = mSpacing;
                break;
            case Graphics::Center:
                textX = getWidth() / 2;
                break;
            case Graphics::Right:
                textX = getWidth() - mSpacing;
                break;
            default:
                throw GCN_EXCEPTION("Unknown alignment.");
        }

        graphics->setFont(getFont());

        if (isPressed())
        {
            graphics->drawText(getCaption(), textX + 1, textY + 1, getAlignment());
        }
        else
        {
            graphics->drawText(getCaption(), textX, textY, getAlignment());

//            if (isFocused())
//            {
//                graphics->drawRectangle(2, 2, getWidth() - 4,
//                                        getHeight() - 4);
//            }
        }
    }

    void ZButton::adjustSize()
    {
        setWidth(getFont()->getWidth(mCaption) + 2*mSpacing);
        setHeight(getFont()->getHeight() + 2*mSpacing);
    }

    bool ZButton::isPressed() const
    {
        if (mMousePressed)
        {
            return mHasMouse;
        }
        else
        {
            return mKeyPressed;
        }
    }

    void ZButton::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.isConsumed())
        {
            return;
        }

        if (mouseEvent.getButton() == MouseEvent::Left)
        {
            mMousePressed = true;
            mouseEvent.consume();
        }
    }

    void ZButton::mouseExited(MouseEvent& mouseEvent)
    {
        mHasMouse = false;
    }

    void ZButton::mouseEntered(MouseEvent& mouseEvent)
    {
        mHasMouse = true;
    }

    void ZButton::mouseReleased(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::Left && mMousePressed && mHasMouse)
        {
            mMousePressed = false;
            distributeActionEvent();
            mouseEvent.consume();
        }
        else if (mouseEvent.getButton() == MouseEvent::Left)
        {
            mMousePressed = false;
            mouseEvent.consume();
        }
    }

    void ZButton::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void ZButton::keyPressed(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();

        if (key.getValue() == Key::Enter
            || key.getValue() == Key::Space)
        {
            mKeyPressed = true;
            keyEvent.consume();
        }
    }

    void ZButton::keyReleased(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();

        if ((key.getValue() == Key::Enter
             || key.getValue() == Key::Space)
            && mKeyPressed)
        {
            mKeyPressed = false;
            distributeActionEvent();
            keyEvent.consume();
        }
    }

    void ZButton::focusLost(const Event& event)
    {
        mMousePressed = false;
        mKeyPressed = false;
    }
}