//
// Created by tatiana on 28.09.18.
//

#include "ZScrollArea.h"
#include "MouseEvent.h"
#include "Graphics.h"
#include "Exception.h"

namespace gui
{
    ZScrollArea::ZScrollArea()
        :ZWidget()
    {

    }

    void ZScrollArea::init()
    {
        ZWidget::init();

        mVScroll = 0;
        mHScroll = 0;
        mHPolicy = ShowAuto;
        mVPolicy = ShowAuto;
        mScrollbarWidth = 12;
        mUpButtonPressed = false;
        mDownButtonPressed = false;
        mLeftButtonPressed = false;
        mRightButtonPressed = false;
        mUpButtonScrollAmount = 10;
        mDownButtonScrollAmount = 10;
        mLeftButtonScrollAmount = 10;
        mRightButtonScrollAmount = 10;
        mIsVerticalMarkerDragged = false;
        mIsHorizontalMarkerDragged =false;
        mOpaque = true;

        addMouseListener(MouseListener::downcasted_shared_from_this<ZScrollArea>());
    }

    void ZScrollArea::init(std::shared_ptr<ZWidget> content)
    {
        ZWidget::init();

        mVScroll = 0;
        mHScroll = 0;
        mHPolicy = ShowAuto;
        mVPolicy = ShowAuto;
        mScrollbarWidth = 12;
        mUpButtonPressed = false;
        mDownButtonPressed = false;
        mLeftButtonPressed = false;
        mRightButtonPressed = false;
        mUpButtonScrollAmount = 10;
        mDownButtonScrollAmount = 10;
        mLeftButtonScrollAmount = 10;
        mRightButtonScrollAmount = 10;
        mIsVerticalMarkerDragged = false;
        mIsHorizontalMarkerDragged =false;
        mOpaque = true;

        setContent(content);
        addMouseListener(MouseListener::downcasted_shared_from_this<ZScrollArea>());
    }

    void ZScrollArea::init(std::shared_ptr<ZWidget> content, ScrollPolicy hPolicy, ScrollPolicy vPolicy)
    {
        ZWidget::init();

        mVScroll = 0;
        mHScroll = 0;
        mHPolicy = hPolicy;
        mVPolicy = vPolicy;
        mScrollbarWidth = 12;
        mUpButtonPressed = false;
        mDownButtonPressed = false;
        mLeftButtonPressed = false;
        mRightButtonPressed = false;
        mUpButtonScrollAmount = 10;
        mDownButtonScrollAmount = 10;
        mLeftButtonScrollAmount = 10;
        mRightButtonScrollAmount = 10;
        mIsVerticalMarkerDragged = false;
        mIsHorizontalMarkerDragged =false;
        mOpaque = true;

        setContent(content);
        addMouseListener(MouseListener::downcasted_shared_from_this<ZScrollArea>());
    }

    ZScrollArea::~ZScrollArea()
    {
        setContent(nullptr);
    }

    void ZScrollArea::setContent(std::shared_ptr<ZWidget> widget)
    {
        if (widget != nullptr)
        {
            clear();
            add_child(widget);
            widget->setPosition(0,0);
        }
        else
        {
            clear();
        }

        checkPolicies();
    }

    std::shared_ptr<ZWidget>  ZScrollArea::getContent() const
    {
        if (mChildren.size() > 0)
            return mChildren.front();

        return nullptr;
    }

    void ZScrollArea::setHorizontalScrollPolicy(ScrollPolicy hPolicy)
    {
        mHPolicy = hPolicy;
        checkPolicies();
    }

    ZScrollArea::ScrollPolicy ZScrollArea::getHorizontalScrollPolicy() const
    {
        return mHPolicy;
    }

    void ZScrollArea::setVerticalScrollPolicy(ScrollPolicy vPolicy)
    {
        mVPolicy = vPolicy;
        checkPolicies();
    }

    ZScrollArea::ScrollPolicy ZScrollArea::getVerticalScrollPolicy() const
    {
        return mVPolicy;
    }

    void ZScrollArea::setScrollPolicy(ScrollPolicy hPolicy, ScrollPolicy vPolicy)
    {
        mHPolicy = hPolicy;
        mVPolicy = vPolicy;
        checkPolicies();
    }

    void ZScrollArea::setVerticalScrollAmount(int vScroll)
    {
        int max = getVerticalMaxScroll();

        mVScroll = vScroll;

        if (vScroll > max)
            mVScroll = max;

        if (vScroll < 0)
            mVScroll = 0;
    }

    int ZScrollArea::getVerticalScrollAmount() const
    {
        return mVScroll;
    }

    void ZScrollArea::setHorizontalScrollAmount(int hScroll)
    {
        int max = getHorizontalMaxScroll();

        mHScroll = hScroll;

        if (hScroll > max)
            mHScroll = max;
        else if (hScroll < 0)
            mHScroll = 0;
    }

    int ZScrollArea::getHorizontalScrollAmount() const
    {
        return mHScroll;
    }

    void ZScrollArea::setScrollAmount(int hScroll, int vScroll)
    {
        setHorizontalScrollAmount(hScroll);
        setVerticalScrollAmount(vScroll);
    }

    int ZScrollArea::getHorizontalMaxScroll()
    {
        checkPolicies();

        if (getContent() == nullptr)
            return 0;

        int value = getContent()->getWidth() - getChildrenArea().width +
                    2 * getContent()->getFrameSize();

        if (value < 0)
            return 0;

        return value;
    }

    int ZScrollArea::getVerticalMaxScroll()
    {
        checkPolicies();

        if (getContent() == nullptr)
            return 0;

        int value;

        value = getContent()->getHeight() - getChildrenArea().height +
                2 * getContent()->getFrameSize();

        if (value < 0)
            return 0;

        return value;
    }

    void ZScrollArea::setScrollbarWidth(int width)
    {
        if (width <= 0)
            throw GCN_EXCEPTION("Width should be greater then 0.");

        mScrollbarWidth = width;
    }

    int ZScrollArea::getScrollbarWidth() const
    {
        return mScrollbarWidth;
    }

    void ZScrollArea::mousePressed(MouseEvent& mouseEvent)
    {
        int x = mouseEvent.getX();
        int y = mouseEvent.getY();

        if (getUpButtonDimension().isContaining(x, y))
        {
            setVerticalScrollAmount(getVerticalScrollAmount() - mUpButtonScrollAmount);
            mUpButtonPressed = true;
        }
        else if (getDownButtonDimension().isContaining(x, y))
        {
            setVerticalScrollAmount(getVerticalScrollAmount() + mDownButtonScrollAmount);
            mDownButtonPressed = true;
        }
        else if (getLeftButtonDimension().isContaining(x, y))
        {
            setHorizontalScrollAmount(getHorizontalScrollAmount() - mLeftButtonScrollAmount);
            mLeftButtonPressed = true;
        }
        else if (getRightButtonDimension().isContaining(x, y))
        {
            setHorizontalScrollAmount(getHorizontalScrollAmount() + mRightButtonScrollAmount);
            mRightButtonPressed = true;
        }
        else if (getVerticalMarkerDimension().isContaining(x, y))
        {
            mIsHorizontalMarkerDragged = false;
            mIsVerticalMarkerDragged = true;

            mVerticalMarkerDragOffset = y - getVerticalMarkerDimension().y;
        }
        else if (getVerticalBarDimension().isContaining(x,y))
        {
            if (y < getVerticalMarkerDimension().y)
            {
                setVerticalScrollAmount(getVerticalScrollAmount() - (int)(getChildrenArea().height * 0.95));
            }
            else
            {
                setVerticalScrollAmount(getVerticalScrollAmount() + (int)(getChildrenArea().height * 0.95));
            }
        }
        else if (getHorizontalMarkerDimension().isContaining(x, y))
        {
            mIsHorizontalMarkerDragged = true;
            mIsVerticalMarkerDragged = false;

            mHorizontalMarkerDragOffset = x - getHorizontalMarkerDimension().x;
        }
        else if (getHorizontalBarDimension().isContaining(x,y))
        {
            if (x < getHorizontalMarkerDimension().x)
            {
                setHorizontalScrollAmount(getHorizontalScrollAmount() - (int)(getChildrenArea().width * 0.95));
            }
            else
            {
                setHorizontalScrollAmount(getHorizontalScrollAmount() + (int)(getChildrenArea().width * 0.95));
            }
        }
    }

    void ZScrollArea::mouseReleased(MouseEvent& mouseEvent)
    {
        mUpButtonPressed = false;
        mDownButtonPressed = false;
        mLeftButtonPressed = false;
        mRightButtonPressed = false;
        mIsHorizontalMarkerDragged = false;
        mIsVerticalMarkerDragged = false;

        mouseEvent.consume();
    }

    void ZScrollArea::mouseDragged(MouseEvent& mouseEvent)
    {
        if (mIsVerticalMarkerDragged)
        {
            int pos = mouseEvent.getY() - getVerticalBarDimension().y  - mVerticalMarkerDragOffset;
            int length = getVerticalMarkerDimension().height;

            Rectangle barDim = getVerticalBarDimension();

            if ((barDim.height - length) > 0)
            {
                setVerticalScrollAmount((getVerticalMaxScroll() * pos) / (barDim.height - length));
            }
            else
            {
                setVerticalScrollAmount(0);
            }
        }

        if (mIsHorizontalMarkerDragged)
        {
            int pos = mouseEvent.getX() - getHorizontalBarDimension().x  - mHorizontalMarkerDragOffset;
            int length = getHorizontalMarkerDimension().width;

            Rectangle barDim = getHorizontalBarDimension();

            if ((barDim.width - length) > 0)
            {
                setHorizontalScrollAmount((getHorizontalMaxScroll() * pos) / (barDim.width - length));
            }
            else
            {
                setHorizontalScrollAmount(0);
            }
        }

        mouseEvent.consume();
    }

    void ZScrollArea::draw_item(std::shared_ptr<Graphics> graphics)
    {
        drawBackground(graphics);

        if (mVBarVisible)
        {
            drawUpButton(graphics);
            drawDownButton(graphics);
            drawVBar(graphics);
            drawVMarker(graphics);
        }

        if (mHBarVisible)
        {
            drawLeftButton(graphics);
            drawRightButton(graphics);
            drawHBar(graphics);
            drawHMarker(graphics);
        }

        if (mHBarVisible && mVBarVisible)
        {
            graphics->setColor(getBaseColor());
            graphics->fillRectangle(getWidth() - mScrollbarWidth, getHeight() - mScrollbarWidth, mScrollbarWidth, mScrollbarWidth);
        }
    }

    void ZScrollArea::drawHBar(std::shared_ptr<Graphics> graphics)
    {
        Rectangle dim = getHorizontalBarDimension();

        graphics->pushClipArea(dim);

        int alpha = getBaseColor().a;
        ZColor trackColor = getBaseColor() - 0x101010;
        trackColor.a = alpha;
        ZColor shadowColor = getBaseColor() - 0x303030;
        shadowColor.a = alpha;

        graphics->setColor(trackColor);
        graphics->fillRectangle(0, 0, dim.width, dim.height);

        graphics->setColor(shadowColor);
        graphics->drawLine(0, 0, dim.width, 0);

        graphics->popClipArea();
    }

    void ZScrollArea::drawVBar(std::shared_ptr<Graphics> graphics)
    {
        Rectangle dim = getVerticalBarDimension();

        graphics->pushClipArea(dim);

        int alpha = getBaseColor().a;
        ZColor trackColor = getBaseColor() - 0x101010;
        trackColor.a = alpha;
        ZColor shadowColor = getBaseColor() - 0x303030;
        shadowColor.a = alpha;

        graphics->setColor(trackColor);
        graphics->fillRectangle(0, 0, dim.width, dim.height);

        graphics->setColor(shadowColor);
        graphics->drawLine(0, 0, 0, dim.height);

        graphics->popClipArea();
    }

    void ZScrollArea::drawBackground(std::shared_ptr<Graphics> graphics)
    {
        if (isOpaque())
        {
            graphics->setColor(getBackgroundColor());
            graphics->fillRectangle(getChildrenArea());
        }
    }

    void ZScrollArea::drawUpButton(std::shared_ptr<Graphics> graphics)
    {
        Rectangle dim = getUpButtonDimension();
        graphics->pushClipArea(dim);

        ZColor highlightColor;
        ZColor shadowColor;
        ZColor faceColor;
        int offset;
        int alpha = getBaseColor().a;

        if (mUpButtonPressed)
        {
            faceColor = getBaseColor() - 0x303030;
            faceColor.a = alpha;
            highlightColor = faceColor - 0x303030;
            highlightColor.a = alpha;
            shadowColor = getBaseColor();
            shadowColor.a = alpha;

            offset = 1;
        }
        else
        {
            faceColor = getBaseColor();
            faceColor.a = alpha;
            highlightColor = faceColor + 0x303030;
            highlightColor.a = alpha;
            shadowColor = faceColor - 0x303030;
            shadowColor.a = alpha;

            offset = 0;
        }

        graphics->setColor(faceColor);
        graphics->fillRectangle(0, 0, dim.width, dim.height);

        graphics->setColor(highlightColor);
        graphics->drawLine(0, 0, dim.width - 1, 0);
        graphics->drawLine(0, 1, 0, dim.height - 1);

        graphics->setColor(shadowColor);
        graphics->drawLine(dim.width - 1, 0, dim.width - 1, dim.height - 1);
        graphics->drawLine(1, dim.height - 1, dim.width - 1, dim.height - 1);

        graphics->setColor(getForegroundColor());

        int i;
        int w = dim.height / 2;
        int h = w / 2 + 2;
        for (i = 0; i < w / 2; ++i)
        {
            graphics->drawLine(w - i + offset, i + h + offset, w + i + offset, i + h + offset);
        }

        graphics->popClipArea();
    }

    void ZScrollArea::drawDownButton(std::shared_ptr<Graphics> graphics)
    {
        Rectangle dim = getDownButtonDimension();
        graphics->pushClipArea(dim);

        ZColor highlightColor;
        ZColor shadowColor;
        ZColor faceColor;
        int offset;
        int alpha = getBaseColor().a;

        if (mDownButtonPressed)
        {
            faceColor = getBaseColor() - 0x303030;
            faceColor.a = alpha;
            highlightColor = faceColor - 0x303030;
            highlightColor.a = alpha;
            shadowColor = getBaseColor();
            shadowColor.a = alpha;

            offset = 1;
        }
        else
        {
            faceColor = getBaseColor();
            faceColor.a = alpha;
            highlightColor = faceColor + 0x303030;
            highlightColor.a = alpha;
            shadowColor = faceColor - 0x303030;
            shadowColor.a = alpha;

            offset = 0;
        }

        graphics->setColor(faceColor);
        graphics->fillRectangle(0, 0, dim.width, dim.height);

        graphics->setColor(highlightColor);
        graphics->drawLine(0, 0, dim.width - 1, 0);
        graphics->drawLine(0, 1, 0, dim.height - 1);

        graphics->setColor(shadowColor);
        graphics->drawLine(dim.width - 1, 0, dim.width - 1, dim.height - 1);
        graphics->drawLine(1, dim.height - 1, dim.width - 1, dim.height - 1);

        graphics->setColor(getForegroundColor());

        int i;
        int w = dim.height / 2;
        int h = w + 1;
        for (i = 0; i < w / 2; ++i)
        {
            graphics->drawLine(w - i + offset, -i + h + offset, w + i + offset, -i + h + offset);
        }

        graphics->popClipArea();
    }

    void ZScrollArea::drawLeftButton(std::shared_ptr<Graphics> graphics)
    {
        Rectangle dim = getLeftButtonDimension();
        graphics->pushClipArea(dim);

        ZColor highlightColor;
        ZColor shadowColor;
        ZColor faceColor;
        int offset;
        int alpha = getBaseColor().a;

        if (mLeftButtonPressed)
        {
            faceColor = getBaseColor() - 0x303030;
            faceColor.a = alpha;
            highlightColor = faceColor - 0x303030;
            highlightColor.a = alpha;
            shadowColor = getBaseColor();
            shadowColor.a = alpha;

            offset = 1;
        }
        else
        {
            faceColor = getBaseColor();
            faceColor.a = alpha;
            highlightColor = faceColor + 0x303030;
            highlightColor.a = alpha;
            shadowColor = faceColor - 0x303030;
            shadowColor.a = alpha;

            offset = 0;
        }

        graphics->setColor(faceColor);
        graphics->fillRectangle(0, 0, dim.width, dim.height);

        graphics->setColor(highlightColor);
        graphics->drawLine(0, 0, dim.width - 1, 0);
        graphics->drawLine(0, 1, 0, dim.height - 1);

        graphics->setColor(shadowColor);
        graphics->drawLine(dim.width - 1, 0, dim.width - 1, dim.height - 1);
        graphics->drawLine(1, dim.height - 1, dim.width - 1, dim.height - 1);

        graphics->setColor(getForegroundColor());

        int i;
        int w = dim.width / 2;
        int h = w - 2;
        for (i = 0; i < w / 2; ++i)
        {
            graphics->drawLine(i + h + offset, w - i + offset, i + h + offset, w + i + offset);
        }

        graphics->popClipArea();
    }

    void ZScrollArea::drawRightButton(std::shared_ptr<Graphics> graphics)
    {
        Rectangle dim = getRightButtonDimension();
        graphics->pushClipArea(dim);

        ZColor highlightColor;
        ZColor shadowColor;
        ZColor faceColor;
        int offset;
        int alpha = getBaseColor().a;

        if (mRightButtonPressed)
        {
            faceColor = getBaseColor() - 0x303030;
            faceColor.a = alpha;
            highlightColor = faceColor - 0x303030;
            highlightColor.a = alpha;
            shadowColor = getBaseColor();
            shadowColor.a = alpha;

            offset = 1;
        }
        else
        {
            faceColor = getBaseColor();
            faceColor.a = alpha;
            highlightColor = faceColor + 0x303030;
            highlightColor.a = alpha;
            shadowColor = faceColor - 0x303030;
            shadowColor.a = alpha;

            offset = 0;
        }

        graphics->setColor(faceColor);
        graphics->fillRectangle(0, 0, dim.width, dim.height);

        graphics->setColor(highlightColor);
        graphics->drawLine(0, 0, dim.width - 1, 0);
        graphics->drawLine(0, 1, 0, dim.height - 1);

        graphics->setColor(shadowColor);
        graphics->drawLine(dim.width - 1, 0, dim.width - 1, dim.height - 1);
        graphics->drawLine(1, dim.height - 1, dim.width - 1, dim.height - 1);

        graphics->setColor(getForegroundColor());

        int i;
        int w = dim.width / 2;
        int h = w + 1;
        for (i = 0; i < w / 2; ++i)
        {
            graphics->drawLine(-i + h + offset, w - i + offset, -i + h + offset, w + i + offset);
        }

        graphics->popClipArea();
    }

    void ZScrollArea::drawVMarker(std::shared_ptr<Graphics> graphics)
    {
        Rectangle dim = getVerticalMarkerDimension();
        graphics->pushClipArea(dim);

        int alpha = getBaseColor().a;
        ZColor faceColor = getBaseColor();
        faceColor.a = alpha;
        ZColor highlightColor = faceColor + 0x303030;
        highlightColor.a = alpha;
        ZColor shadowColor = faceColor - 0x303030;
        shadowColor.a = alpha;

        graphics->setColor(faceColor);
        graphics->fillRectangle(1, 1, dim.width - 1, dim.height - 1);

        graphics->setColor(highlightColor);
        graphics->drawLine(0, 0, dim.width - 1, 0);
        graphics->drawLine(0, 1, 0, dim.height - 1);

        graphics->setColor(shadowColor);
        graphics->drawLine(1, dim.height - 1, dim.width - 1, dim.height - 1);
        graphics->drawLine(dim.width - 1, 0, dim.width - 1, dim.height - 1);

        graphics->popClipArea();
    }

    void ZScrollArea::drawHMarker(std::shared_ptr<Graphics> graphics)
    {
        Rectangle dim = getHorizontalMarkerDimension();
        graphics->pushClipArea(dim);

        int alpha = getBaseColor().a;
        ZColor faceColor = getBaseColor();
        faceColor.a = alpha;
        ZColor highlightColor = faceColor + 0x303030;
        highlightColor.a = alpha;
        ZColor shadowColor = faceColor - 0x303030;
        shadowColor.a = alpha;

        graphics->setColor(faceColor);
        graphics->fillRectangle(1, 1, dim.width - 1, dim.height - 1);

        graphics->setColor(highlightColor);
        graphics->drawLine(0, 0, dim.width - 1, 0);
        graphics->drawLine(0, 1, 0, dim.height - 1);

        graphics->setColor(shadowColor);
        graphics->drawLine(1, dim.height - 1, dim.width - 1, dim.height - 1);
        graphics->drawLine(dim.width - 1, 0, dim.width - 1, dim.height - 1);

        graphics->popClipArea();
    }

    void ZScrollArea::logic_item()
    {
        checkPolicies();

        setVerticalScrollAmount(getVerticalScrollAmount());
        setHorizontalScrollAmount(getHorizontalScrollAmount());

        if (getContent() != nullptr)
        {
            getContent()->setPosition(-mHScroll + getContent()->getFrameSize(), -mVScroll + getContent()->getFrameSize());
            getContent()->logic_item();
        }
    }

    void ZScrollArea::checkPolicies()
    {
        int w = getWidth();
        int h = getHeight();

        mHBarVisible = false;
        mVBarVisible = false;

        if (!getContent())
        {
            mHBarVisible = (mHPolicy == ShowAlways);
            mVBarVisible = (mVPolicy == ShowAlways);
            return;
        }

        if (mHPolicy == ShowAuto && mVPolicy == ShowAuto)
        {
            if (getContent()->getWidth() <= w && getContent()->getHeight() <= h)
            {
                mHBarVisible = false;
                mVBarVisible = false;
            }

            if (getContent()->getWidth() > w)
                mHBarVisible = true;

            if ((getContent()->getHeight() > h) || (mHBarVisible && getContent()->getHeight() > h - mScrollbarWidth))
            {
                mVBarVisible = true;
            }

            if (mVBarVisible && getContent()->getWidth() > w - mScrollbarWidth)
                mHBarVisible = true;

            return;
        }

        switch (mHPolicy)
        {
            case ShowNever:
                mHBarVisible = false;
                break;

            case ShowAlways:
                mHBarVisible = true;
                break;

            case ShowAuto:
                if (mVPolicy == ShowNever)
                {
                    mHBarVisible = getContent()->getWidth() > w;
                }
                else // (mVPolicy == ShowAlways)
                {
                    mHBarVisible = getContent()->getWidth() > w - mScrollbarWidth;
                }
                break;

            default:
                throw GCN_EXCEPTION("Horizontal scroll policy invalid.");
        }

        switch (mVPolicy)
        {
            case ShowNever:
                mVBarVisible = false;
                break;

            case ShowAlways:
                mVBarVisible = true;
                break;

            case ShowAuto:
                if (mHPolicy == ShowNever)
                {
                    mVBarVisible = getContent()->getHeight() > h;
                }
                else // (mHPolicy == ShowAlways)
                {
                    mVBarVisible = getContent()->getHeight() > h - mScrollbarWidth;
                }
                break;
            default:
                throw GCN_EXCEPTION("Vertical scroll policy invalid.");
        }
    }

    Rectangle ZScrollArea::getUpButtonDimension()
    {
        if (!mVBarVisible)
            return Rectangle(0, 0, 0, 0);

        return Rectangle(getWidth() - mScrollbarWidth, 0, mScrollbarWidth, mScrollbarWidth);
    }

    Rectangle ZScrollArea::getDownButtonDimension()
    {
        if (!mVBarVisible)
            return Rectangle(0, 0, 0, 0);

        if (mVBarVisible && mHBarVisible)
        {
            return Rectangle(getWidth() - mScrollbarWidth, getHeight() - mScrollbarWidth*2, mScrollbarWidth, mScrollbarWidth);
        }

        return Rectangle(getWidth() - mScrollbarWidth, getHeight() - mScrollbarWidth, mScrollbarWidth, mScrollbarWidth);
    }

    Rectangle ZScrollArea::getLeftButtonDimension()
    {
        if (!mHBarVisible)
            return Rectangle(0, 0, 0, 0);

        return Rectangle(0, getHeight() - mScrollbarWidth, mScrollbarWidth, mScrollbarWidth);
    }

    Rectangle ZScrollArea::getRightButtonDimension()
    {
        if (!mHBarVisible)
            return Rectangle(0, 0, 0, 0);

        if (mVBarVisible && mHBarVisible)
        {
            return Rectangle(getWidth() - mScrollbarWidth*2, getHeight() - mScrollbarWidth, mScrollbarWidth, mScrollbarWidth);
        }

        return Rectangle(getWidth() - mScrollbarWidth, getHeight() - mScrollbarWidth, mScrollbarWidth, mScrollbarWidth);
    }

    Rectangle ZScrollArea::getChildrenArea()
    {
        Rectangle area = Rectangle(0, 0, mVBarVisible ? getWidth() - mScrollbarWidth : getWidth(),
                mHBarVisible ? getHeight() - mScrollbarWidth : getHeight());

        if (area.isEmpty())
            return Rectangle();

        return area;
    }

    Rectangle ZScrollArea::getVerticalBarDimension()
    {
        if (!mVBarVisible)
            return Rectangle(0, 0, 0, 0);

        if (mHBarVisible)
        {
            return Rectangle(getWidth() - mScrollbarWidth,
                             getUpButtonDimension().height,
                             mScrollbarWidth,
                             getHeight()
                             - getUpButtonDimension().height
                             - getDownButtonDimension().height
                             - mScrollbarWidth);
        }

        return Rectangle(getWidth() - mScrollbarWidth,
                         getUpButtonDimension().height,
                         mScrollbarWidth,
                         getHeight()
                         - getUpButtonDimension().height
                         - getDownButtonDimension().height);
    }

    Rectangle ZScrollArea::getHorizontalBarDimension()
    {
        if (!mHBarVisible)
            return Rectangle(0, 0, 0, 0);

        if (mVBarVisible)
        {
            return Rectangle(getLeftButtonDimension().width,
                             getHeight() - mScrollbarWidth,
                             getWidth()
                             - getLeftButtonDimension().width
                             - getRightButtonDimension().width
                             - mScrollbarWidth,
                             mScrollbarWidth);
        }

        return Rectangle(getLeftButtonDimension().width,
                         getHeight() - mScrollbarWidth,
                         getWidth()
                         - getLeftButtonDimension().width
                         - getRightButtonDimension().width,
                         mScrollbarWidth);
    }

    Rectangle ZScrollArea::getVerticalMarkerDimension()
    {
        if (!mVBarVisible)
            return Rectangle(0, 0, 0, 0);

        int length, pos;
        Rectangle barDim = getVerticalBarDimension();

        if (getContent() && getContent()->getHeight() != 0)
        {
            length = (barDim.height * getChildrenArea().height) / getContent()->getHeight();
        }
        else
        {
            length = barDim.height;
        }

        if (length < mScrollbarWidth)
            length = mScrollbarWidth;

        if (length > barDim.height)
            length = barDim.height;

        if (getVerticalMaxScroll() != 0)
        {
            pos = ((barDim.height - length) * getVerticalScrollAmount()) / getVerticalMaxScroll();
        }
        else
        {
            pos = 0;
        }

        return Rectangle(barDim.x, barDim.y + pos, mScrollbarWidth, length);
    }

    Rectangle ZScrollArea::getHorizontalMarkerDimension()
    {
        if (!mHBarVisible)
            return Rectangle(0, 0, 0, 0);

        int length, pos;
        Rectangle barDim = getHorizontalBarDimension();

        if (getContent() && getContent()->getWidth() != 0)
        {
            length = (barDim.width * getChildrenArea().width) / getContent()->getWidth();
        }
        else
        {
            length = barDim.width;
        }

        if (length < mScrollbarWidth)
            length = mScrollbarWidth;

        if (length > barDim.width)
            length = barDim.width;

        if (getHorizontalMaxScroll() != 0)
        {
            pos = ((barDim.width - length) * getHorizontalScrollAmount()) / getHorizontalMaxScroll();
        }
        else
        {
            pos = 0;
        }

        return Rectangle(barDim.x + pos, barDim.y, length, mScrollbarWidth);
    }

    void ZScrollArea::showWidgetPart(std::shared_ptr<ZWidget> widget, Rectangle area)
    {
        if (widget != getContent())
            throw GCN_EXCEPTION("Widget not content widget");

        ZWidget::showWidgetPart(widget, area);

        setHorizontalScrollAmount(getContent()->getFrameSize() - getContent()->getX());
        setVerticalScrollAmount(getContent()->getFrameSize() - getContent()->getY());
    }

    std::shared_ptr<ZWidget> ZScrollArea::getWidgetAt(int x, int y)
    {
        if (getChildrenArea().isContaining(x, y))
            return getContent();

        return nullptr;
    }

    void ZScrollArea::mouseWheelMovedUp(MouseEvent& mouseEvent)
    {
        if (mouseEvent.isConsumed())
            return;

        setVerticalScrollAmount(getVerticalScrollAmount() - getChildrenArea().height / 8);

        mouseEvent.consume();
    }

    void ZScrollArea::mouseWheelMovedDown(MouseEvent& mouseEvent)
    {
        if (mouseEvent.isConsumed())
            return;

        setVerticalScrollAmount(getVerticalScrollAmount() + getChildrenArea().height / 8);

        mouseEvent.consume();
    }

    void ZScrollArea::setWidth(int width)
    {
        ZWidget::setWidth(width);
        checkPolicies();
    }

    void ZScrollArea::setHeight(int height)
    {
        ZWidget::setHeight(height);
        checkPolicies();
    }

    void ZScrollArea::setDimension(const Rectangle& dimension)
    {
        ZWidget::setDimension(dimension);
        checkPolicies();
    }

    void ZScrollArea::setLeftButtonScrollAmount(int amount)
    {
        mLeftButtonScrollAmount = amount;
    }

    void ZScrollArea::setRightButtonScrollAmount(int amount)
    {
        mRightButtonScrollAmount = amount;
    }

    void ZScrollArea::setUpButtonScrollAmount(int amount)
    {
        mUpButtonScrollAmount = amount;
    }

    void ZScrollArea::setDownButtonScrollAmount(int amount)
    {
        mDownButtonScrollAmount = amount;
    }

    int ZScrollArea::getLeftButtonScrollAmount() const
    {
        return mLeftButtonScrollAmount;
    }

    int ZScrollArea::getRightButtonScrollAmount() const
    {
        return mRightButtonScrollAmount;
    }

    int ZScrollArea::getUpButtonScrollAmount() const
    {
        return mUpButtonScrollAmount;
    }

    int ZScrollArea::getDownButtonScrollAmount() const
    {
        return mDownButtonScrollAmount;
    }

    void ZScrollArea::setOpaque(bool opaque)
    {
        mOpaque = opaque;
    }

    bool ZScrollArea::isOpaque() const
    {
        return mOpaque;
    }
}