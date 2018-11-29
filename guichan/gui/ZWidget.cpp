//
// Created by tatiana on 26.09.18.
//

#include "ZWidget.h"
#include "Event.h"
#include "ActionEvent.h"
#include "Graphics.h"
#include "FocusHandler.h"
#include "DeathListener.h"
#include "WidgetListener.h"
#include "ActionListener.h"
#include "Exception.h"
#include "ZListBox.h"

#include <algorithm>

namespace gui
{
    std::shared_ptr<Font> ZWidget::mGlobalFont = nullptr;
    std::shared_ptr<DefaultFont> ZWidget::mDefaultFont = std::shared_ptr<DefaultFont>(new DefaultFont);
    std::list<std::shared_ptr<ZWidget>> ZWidget::mWidgetInstances;

    ZWidget::ZWidget()
    {

    }

    void ZWidget::init()
    {
        mForegroundColor=0x000000;
        mBackgroundColor=0xffffff;
        mBaseColor=0x808090;
        mSelectionColor=0xc3d9ff;
        mFocusHandler=nullptr;
        mInternalFocusHandler=nullptr;
        mParent=nullptr;
        mFrameSize=0;
        mFocusable=false;
        mVisible=true;
        mTabIn=true;
        mTabOut=true;
        mEnabled=true;
        mCurrentFont=nullptr;

        mWidgetInstances.push_back(shared_from_this());
    }

    ZWidget::~ZWidget()
    {

    }

    void ZWidget::shutdown()
    {
        if (mParent != nullptr)
            mParent->remove(shared_from_this());

        for (auto item : mChildren)
            item->setParent(nullptr);

        for (auto item : mDeathListeners)
        {
            Event event(shared_from_this());
            item->death(event);
        }

        setFocusHandler(nullptr);

        mWidgetInstances.remove(shared_from_this());
    }

    void ZWidget::drawFrame(std::shared_ptr<Graphics> graphics)
    {
        ZColor faceColor = getBaseColor();
        ZColor highlightColor, shadowColor;
        int alpha = getBaseColor().a;
        int width = getWidth() + getFrameSize() * 2 - 1;
        int height = getHeight() + getFrameSize() * 2 - 1;
        highlightColor = faceColor + 0x303030;
        highlightColor.a = alpha;
        shadowColor = faceColor - 0x303030;
        shadowColor.a = alpha;

        unsigned int i;
        for (i = 0; i < getFrameSize(); ++i)
        {
            graphics->setColor(shadowColor);
            graphics->drawLine(i,i, width - i, i);
            graphics->drawLine(i,i + 1, i, height - i - 1);
            graphics->setColor(highlightColor);
            graphics->drawLine(width - i,i + 1, width - i, height - i);
            graphics->drawLine(i,height - i, width - i - 1, height - i);
        }
    }

    void ZWidget::setParent(std::shared_ptr<ZWidget> parent)
    {
        mParent = parent;
    }

    std::shared_ptr<ZWidget> ZWidget::getParent() const
    {
        return mParent;
    }

    void ZWidget::setWidth(int width)
    {
        Rectangle newDimension = mDimension;
        newDimension.width = width;

        setDimension(newDimension);
    }

    int ZWidget::getWidth() const
    {
        return mDimension.width;
    }

    void ZWidget::setHeight(int height)
    {
        Rectangle newDimension = mDimension;
        newDimension.height = height;

        setDimension(newDimension);
    }

    int ZWidget::getHeight() const
    {
        return mDimension.height;
    }

    void ZWidget::setX(int x)
    {
        Rectangle newDimension = mDimension;
        newDimension.x = x;

        setDimension(newDimension);
    }

    int ZWidget::getX() const
    {
        return mDimension.x;
    }

    void ZWidget::setY(int y)
    {
        Rectangle newDimension = mDimension;
        newDimension.y = y;

        setDimension(newDimension);
    }

    int ZWidget::getY() const
    {
        return mDimension.y;
    }

    void ZWidget::setPosition(int x, int y)
    {
        Rectangle newDimension = mDimension;
        newDimension.x = x;
        newDimension.y = y;

        setDimension(newDimension);
    }

    void ZWidget::setDimension(const Rectangle& dimension)
    {
        Rectangle oldDimension = mDimension;
        mDimension = dimension;

        if (mDimension.width != oldDimension.width
            || mDimension.height != oldDimension.height)
        {
            distributeResizedEvent();
        }

        if (mDimension.x != oldDimension.x
            || mDimension.y != oldDimension.y)
        {
            distributeMovedEvent();
        }
    }

    void ZWidget::setFrameSize(unsigned int frameSize)
    {
        mFrameSize = frameSize;
    }

    unsigned int ZWidget::getFrameSize() const
    {
        return mFrameSize;
    }

    const Rectangle& ZWidget::getDimension() const
    {
        return mDimension;
    }

    const std::string& ZWidget::getActionEventId() const
    {
        return mActionEventId;
    }

    void ZWidget::setActionEventId(const std::string& actionEventId)
    {
        mActionEventId = actionEventId;
    }

    bool ZWidget::isFocused() const
    {
        if (!mFocusHandler)
        {
            return false;
        }

        return (mFocusHandler->isFocused(shared_from_this()));
    }

    void ZWidget::setFocusable(bool focusable)
    {
        if (!focusable && isFocused())
        {
            mFocusHandler->focusNone();
        }

        mFocusable = focusable;
    }

    bool ZWidget::isFocusable() const
    {
        return mFocusable && isVisible() && isEnabled();
    }

    void ZWidget::requestFocus()
    {
        if (mFocusHandler == nullptr)
            throw GCN_EXCEPTION("No focushandler set (did you add the widget to the gui?).");

        if (isFocusable())
            mFocusHandler->requestFocus(shared_from_this());
    }

    void ZWidget::requestMoveToTop()
    {
        if (mParent != nullptr)
            mParent->moveToTop(shared_from_this());
    }

    void ZWidget::requestMoveToBottom()
    {
        if (mParent != nullptr)
            mParent->moveToBottom(shared_from_this());
    }

    void ZWidget::setVisible(bool visible)
    {
        if (!visible && isFocused())
            mFocusHandler->focusNone();

        if (visible)
            distributeShownEvent();
        else if(!visible)
            distributeHiddenEvent();

        mVisible = visible;
    }

    bool ZWidget::isVisible() const
    {
        if (getParent() == nullptr)
            return mVisible;
        else
            return mVisible && getParent()->isVisible();
    }

    void ZWidget::setBaseColor(const ZColor& color)
    {
        mBaseColor = color;
    }

    const ZColor& ZWidget::getBaseColor() const
    {
        return mBaseColor;
    }

    void ZWidget::setForegroundColor(const ZColor& color)
    {
        mForegroundColor = color;
    }

    const ZColor& ZWidget::getForegroundColor() const
    {
        return mForegroundColor;
    }

    void ZWidget::setBackgroundColor(const ZColor& color)
    {
        mBackgroundColor = color;
    }

    const ZColor& ZWidget::getBackgroundColor() const
    {
        return mBackgroundColor;
    }

    void ZWidget::setSelectionColor(const ZColor& color)
    {
        mSelectionColor = color;
    }

    const ZColor& ZWidget::getSelectionColor() const
    {
        return mSelectionColor;
    }

    void ZWidget::setFocusHandler(std::shared_ptr<FocusHandler> focusHandler)
    {
        if (mFocusHandler)
        {
            releaseModalFocus();
            mFocusHandler->remove(shared_from_this());
        }

        if (focusHandler)
            focusHandler->add(shared_from_this());

        mFocusHandler = focusHandler;

        if (mInternalFocusHandler != nullptr)
            return;

        for (auto item : mChildren) {
            if (widgetExists(item))
                item->setFocusHandler(focusHandler);
        }
    }

    std::shared_ptr<FocusHandler> ZWidget::getFocusHandler()
    {
        return mFocusHandler;
    }

    void ZWidget::addActionListener(std::shared_ptr<ActionListener> actionListener)
    {
        mActionListeners.push_back(actionListener);
    }

    void ZWidget::removeActionListener(std::shared_ptr<ActionListener> actionListener)
    {
        mActionListeners.remove(actionListener);
    }

    void ZWidget::addDeathListener(std::shared_ptr<DeathListener> deathListener)
    {
        mDeathListeners.push_back(deathListener);
    }

    void ZWidget::removeDeathListener(std::shared_ptr<DeathListener> deathListener)
    {
        mDeathListeners.remove(deathListener);
    }

    void ZWidget::addKeyListener(std::shared_ptr<KeyListener> keyListener)
    {
        mKeyListeners.push_back(keyListener);
    }

    void ZWidget::removeKeyListener(std::shared_ptr<KeyListener> keyListener)
    {
        mKeyListeners.remove(keyListener);
    }

    void ZWidget::addFocusListener(std::shared_ptr<FocusListener> focusListener)
    {
        mFocusListeners.push_back(focusListener);
    }

    void ZWidget::removeFocusListener(std::shared_ptr<FocusListener> focusListener)
    {
        mFocusListeners.remove(focusListener);
    }

    void ZWidget::addMouseListener(std::shared_ptr<MouseListener> mouseListener)
    {
        mMouseListeners.push_back(mouseListener);
    }

    void ZWidget::removeMouseListener(std::shared_ptr<MouseListener> mouseListener)
    {
        mMouseListeners.remove(mouseListener);
    }

    void ZWidget::addWidgetListener(std::shared_ptr<WidgetListener> widgetListener)
    {
        mWidgetListeners.push_back(widgetListener);
    }

    void ZWidget::removeWidgetListener(std::shared_ptr<WidgetListener> widgetListener)
    {
        mWidgetListeners.remove(widgetListener);
    }

    void ZWidget::getAbsolutePosition(int& x, int& y) const
    {
        if (getParent() == nullptr)
        {
            x = mDimension.x;
            y = mDimension.y;
            return;
        }

        int parentX;
        int parentY;

        getParent()->getAbsolutePosition(parentX, parentY);

        x = parentX + mDimension.x + getParent()->getChildrenArea().x;
        y = parentY + mDimension.y + getParent()->getChildrenArea().y;
    }

    std::shared_ptr<Font> ZWidget::getFont() const
    {
        if (mCurrentFont == nullptr)
        {
            if (mGlobalFont == nullptr)
                return mDefaultFont;

            return mGlobalFont;
        }

        return mCurrentFont;
    }

    void ZWidget::setGlobalFont(std::shared_ptr<Font> font)
    {
        mGlobalFont = font;

        for (auto item : mWidgetInstances)
        {
            if (item->mCurrentFont==nullptr)
                item->fontChanged();
        }
    }

    void ZWidget::setFont(std::shared_ptr<Font> font)
    {
        mCurrentFont = font;
        fontChanged();
    }

    bool ZWidget::widgetExists(std::shared_ptr<const ZWidget> widget)
    {
        for (auto item : mWidgetInstances)
        {
            if (item==widget)
                return true;
        }

        return false;
    }

    bool ZWidget::isTabInEnabled() const
    {
        return mTabIn;
    }

    void ZWidget::setTabInEnabled(bool enabled)
    {
        mTabIn = enabled;
    }

    bool ZWidget::isTabOutEnabled() const
    {
        return mTabOut;
    }

    void ZWidget::setTabOutEnabled(bool enabled)
    {
        mTabOut = enabled;
    }

    void ZWidget::setSize(int width, int height)
    {
        Rectangle newDimension = mDimension;
        newDimension.width = width;
        newDimension.height = height;

        setDimension(newDimension);
    }

    void ZWidget::setEnabled(bool enabled)
    {
        mEnabled = enabled;
    }

    bool ZWidget::isEnabled() const
    {
        return mEnabled && isVisible();
    }

    void ZWidget::requestModalFocus()
    {
        if (mFocusHandler == nullptr)
            throw GCN_EXCEPTION("No focushandler set (did you add the widget to the gui?).");

        mFocusHandler->requestModalFocus(shared_from_this());
    }

    void ZWidget::requestModalMouseInputFocus()
    {
        if (mFocusHandler == nullptr)
            throw GCN_EXCEPTION("No focushandler set (did you add the widget to the gui?).");

        mFocusHandler->requestModalMouseInputFocus(shared_from_this());
    }

    void ZWidget::releaseModalFocus()
    {
        if (mFocusHandler == nullptr)
            return;

        mFocusHandler->releaseModalFocus(shared_from_this());
    }

    void ZWidget::releaseModalMouseInputFocus()
    {
        if (mFocusHandler == nullptr)
            return;

        mFocusHandler->releaseModalMouseInputFocus(shared_from_this());
    }

    bool ZWidget::isModalFocused() const
    {
        if (mFocusHandler == nullptr)
            throw GCN_EXCEPTION("No focushandler set (did you add the widget to the gui?).");

        if (getParent() != nullptr)
        {
            return (mFocusHandler->getModalFocused() == shared_from_this()) || getParent()->isModalFocused();
        }

        return mFocusHandler->getModalFocused() == shared_from_this();
    }

    bool ZWidget::isModalMouseInputFocused() const
    {
        if (mFocusHandler == nullptr)
            throw GCN_EXCEPTION("No focushandler set (did you add the widget to the gui?).");

        if (getParent() != nullptr)
        {
            return (mFocusHandler->getModalMouseInputFocused() == shared_from_this()) || getParent()->isModalMouseInputFocused();
        }

        return mFocusHandler->getModalMouseInputFocused() == shared_from_this();
    }

    std::shared_ptr<ZWidget> ZWidget::getWidgetAt(int x, int y)
    {
        Rectangle r = getChildrenArea();

        if (!r.isContaining(x, y))
            return nullptr;

        x -= r.x;
        y -= r.y;

        for (auto item : mChildren)
        {
            if (item->isVisible() && item->getDimension().isContaining(x, y))
                return item;
        }

        return nullptr;
    }

    const std::list<std::shared_ptr<MouseListener>>& ZWidget::getMouseListeners()
    {
        return mMouseListeners;
    }

    const std::list<std::shared_ptr<KeyListener>>& ZWidget::getKeyListeners()
    {
        return mKeyListeners;
    }

    const std::list<std::shared_ptr<FocusListener>>& ZWidget::getFocusListeners()
    {
        return mFocusListeners;
    }

    Rectangle ZWidget::getChildrenArea()
    {
        return Rectangle(0, 0, 0, 0);
    }

    std::shared_ptr<FocusHandler> ZWidget::getInternalFocusHandler()
    {
        return mInternalFocusHandler;
    }

    void ZWidget::setInternalFocusHandler(std::shared_ptr<FocusHandler> focusHandler)
    {
        mInternalFocusHandler = focusHandler;

        for (auto item : mChildren)
        {
            if (mInternalFocusHandler == nullptr)
                item->setFocusHandler(getFocusHandler());
            else
                item->setFocusHandler(mInternalFocusHandler);
        }
    }

    void ZWidget::setId(const std::string& id)
    {
        mId = id;
    }

    const std::string& ZWidget::getId() const
    {
        return mId;
    }

    void ZWidget::distributeResizedEvent()
    {
        for (auto listener : mWidgetListeners)
        {
            Event event(shared_from_this());
            listener->widgetResized(event);
        }
    }

    void ZWidget::distributeMovedEvent()
    {
        for (auto listener : mWidgetListeners)
        {
            Event event(shared_from_this());
            listener->widgetMoved(event);
        }
    }

    void ZWidget::distributeHiddenEvent()
    {
        for (auto listener : mWidgetListeners)
        {
            Event event(shared_from_this());
            listener->widgetHidden(event);
        }
    }

    void ZWidget::distributeShownEvent()
    {
        for (auto listener : mWidgetListeners)
        {
            Event event(shared_from_this());
            listener->widgetShown(event);
        }
    }

    void ZWidget::distributeActionEvent()
    {
        for (auto listener : mActionListeners)
        {
            ActionEvent actionEvent(shared_from_this(), mActionEventId);
            listener->action(actionEvent);
        }
    }

    void ZWidget::showPart(Rectangle rectangle)
    {
        if (mParent != nullptr)
            mParent->showWidgetPart(shared_from_this(), rectangle);
    }

    std::shared_ptr<ZWidget> ZWidget::getTop() const
    {
        if (getParent() == nullptr)
            return nullptr;

        auto widget = getParent();
        auto parent = getParent()->getParent();

        while (parent != nullptr)
        {
            widget = parent;
            parent = parent->getParent();
        }

        return widget;
    }

    std::list<std::shared_ptr<ZWidget>> ZWidget::getWidgetsIn(const Rectangle& area, std::shared_ptr<ZWidget> ignore)
    {
        std::list<std::shared_ptr<ZWidget>> result;

        for (auto item : mChildren)
        {
            if (ignore != item && item->getDimension().isIntersecting(area))
                result.push_back(item);
        }

        return result;
    }

    void ZWidget::resizeToChildren()
    {
        int w = 0, h = 0;
        for (auto item : mChildren)
        {
            if (item->getX() + item->getWidth() > w)
                w = item->getX() + item->getWidth();

            if (item->getY() + item->getHeight() > h)
                h = item->getY() + item->getHeight();
        }

        setSize(w, h);
    }

    std::shared_ptr<ZWidget> ZWidget::findWidgetById(const std::string& id)
    {
        for (auto item : mChildren)
        {
            if (item->getId() == id)
                return item;

            auto child = item->findWidgetById(id);

            if (child != nullptr)
                return child;
        }

        return nullptr;
    }

    void ZWidget::showWidgetPart(std::shared_ptr<ZWidget> widget, Rectangle area)
    {
        Rectangle widgetArea = getChildrenArea();

        area.x += widget->getX();
        area.y += widget->getY();

        if (area.x + area.width > widgetArea.width)
            widget->setX(widget->getX() - area.x - area.width + widgetArea.width);

        if (area.y + area.height > widgetArea.height)
            widget->setY(widget->getY() - area.y - area.height + widgetArea.height);

        if (area.x < 0)
            widget->setX(widget->getX() - area.x);

        if (area.y < 0)
            widget->setY(widget->getY() - area.y);
    }

    void ZWidget::clear()
    {
        for (auto item : mChildren)
        {
            item->setFocusHandler(nullptr);
            item->setParent(nullptr);
        }

        mChildren.clear();
    }

    void ZWidget::remove(std::shared_ptr<ZWidget> widget)
    {
        auto iter=std::find_if(mChildren.begin(),mChildren.end(),[&widget](std::shared_ptr<ZWidget>& item)
        {
            return widget==item;
        });

        if (iter!=mChildren.end())
        {
            mChildren.erase(iter);
            widget->setFocusHandler(nullptr);
            widget->setParent(nullptr);
            return;
        }

        throw GCN_EXCEPTION("There is no such widget in this container.");
    }

    void ZWidget::add_child(std::shared_ptr<ZWidget> widget)
    {
        mChildren.push_back(widget);

        if (mInternalFocusHandler == nullptr)
            widget->setFocusHandler(getFocusHandler());
        else
            widget->setFocusHandler(mInternalFocusHandler);

        widget->setParent(shared_from_this());
    }

    void ZWidget::moveToTop(std::shared_ptr<ZWidget> widget)
    {
        auto iter=std::find_if(mChildren.begin(),mChildren.end(),[&widget](std::shared_ptr<ZWidget>& item)
        {
            return widget==item;
        });

        if (iter == mChildren.end())
            throw GCN_EXCEPTION("There is no such widget in this widget.");

        mChildren.remove(widget);
        mChildren.push_back(widget);
    }

    void ZWidget::moveToBottom(std::shared_ptr<ZWidget> widget)
    {
        auto iter=std::find_if(mChildren.begin(),mChildren.end(),[&widget](std::shared_ptr<ZWidget>& item)
        {
            return widget==item;
        });

        if (iter == mChildren.end())
            throw GCN_EXCEPTION("There is no such widget in this widget.");

        mChildren.remove(widget);
        mChildren.push_front(widget);
    }

    void ZWidget::focusNext()
    {
        auto iter=std::find_if(mChildren.begin(),mChildren.end(),[](std::shared_ptr<ZWidget>& item)
        {
            return item->isFocused();
        });

        auto end = iter;

        if (iter == mChildren.end())
            iter = mChildren.begin();

        iter++;

        iter=std::find_if(iter,end,[](std::shared_ptr<ZWidget>& item)
        {
            return item->isFocusable();
        });

        if (iter!=end)
        {
            iter->get()->requestFocus();
        }
    }

    void ZWidget::focusPrevious()
    {
        auto iter=std::find_if(mChildren.rbegin(),mChildren.rend(),[](std::shared_ptr<ZWidget>& item)
        {
            return item->isFocused();
        });

        auto end = iter;

        iter++;

        if (iter == mChildren.rend())
            iter = mChildren.rbegin();

        iter=std::find_if(iter,end,[](std::shared_ptr<ZWidget>& item)
        {
            return item->isFocusable();
        });

        if (iter!=end)
        {
            iter->get()->requestFocus();
        }
    }

    void ZWidget::logic_item()
    {

    }

    void ZWidget::draw(std::shared_ptr<Graphics> graphics)
    {
        if (mFrameSize > 0)
        {
            Rectangle rec = mDimension;
            rec.x -= mFrameSize;
            rec.y -= mFrameSize;
            rec.width += 2 * mFrameSize;
            rec.height += 2 * mFrameSize;
            graphics->pushClipArea(rec);
            drawFrame(graphics);
            graphics->popClipArea();
        }

        graphics->pushClipArea(mDimension);
        draw_item(graphics);

        const Rectangle& childrenArea = getChildrenArea();
        graphics->pushClipArea(childrenArea);

        for (auto widget : mChildren)
        {
            // Only draw a widget if it's visible and if it visible
            // inside the children area.
            if (widget->isVisible() && childrenArea.isIntersecting(widget->getDimension()))
                widget->draw(graphics);
        }

        graphics->popClipArea();
        graphics->popClipArea();
    }

    void ZWidget::logic()
    {
        logic_item();

        for (auto widget : mChildren)
            widget->logic();
    }

    const std::list<std::shared_ptr<ZWidget>>& ZWidget::getChildren() const
    {
        return mChildren;
    }
}