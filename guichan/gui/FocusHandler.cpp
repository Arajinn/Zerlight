//
// Created by tatiana on 26.09.18.
//

#include "FocusHandler.h"
#include "ZWidget.h"
#include "Event.h"
#include "FocusListener.h"
#include "Exception.h"
#include <algorithm>

namespace gui
{
    FocusHandler::FocusHandler()
            :mFocusedWidget(nullptr),
             mModalFocusedWidget(nullptr),
             mModalMouseInputFocusedWidget(nullptr),
             mDraggedWidget(nullptr),
             mLastWidgetWithMouse(nullptr),
             mLastWidgetWithModalFocus(nullptr),
             mLastWidgetWithModalMouseInputFocus(nullptr),
             mLastWidgetPressed(nullptr)
    {

    }

    FocusHandler::~FocusHandler()
    {

    }

    void FocusHandler::requestFocus(std::shared_ptr<ZWidget> widget)
    {
        if (widget == nullptr || widget == mFocusedWidget)
        {
            return;
        }

        unsigned int i = 0;
        int toBeFocusedIndex = -1;
        for (i = 0; i < mWidgets.size(); ++i)
        {
            if (mWidgets[i] == widget)
            {
                toBeFocusedIndex = i;
                break;
            }
        }

        if (toBeFocusedIndex < 0)
        {
            throw GCN_EXCEPTION("Trying to focus a none existing widget.");
        }

        auto oldFocused = mFocusedWidget;

        if (oldFocused != widget)
        {
            mFocusedWidget = mWidgets.at(toBeFocusedIndex);

            if (oldFocused != nullptr)
            {
                Event focusEvent(oldFocused);
                distributeFocusLostEvent(focusEvent);
            }

            Event focusEvent(mWidgets.at(toBeFocusedIndex));
            distributeFocusGainedEvent(focusEvent);
        }
    }

    void FocusHandler::requestModalFocus(std::shared_ptr<ZWidget> widget)
    {
        if (mModalFocusedWidget != nullptr && mModalFocusedWidget != widget)
        {
            throw GCN_EXCEPTION("Another widget already has modal focus.");
        }

        mModalFocusedWidget = widget;

        if (mFocusedWidget != nullptr && !mFocusedWidget->isModalFocused())
        {
            focusNone();
        }
    }

    void FocusHandler::requestModalMouseInputFocus(std::shared_ptr<ZWidget> widget)
    {
        if (mModalMouseInputFocusedWidget != nullptr
            && mModalMouseInputFocusedWidget != widget)
        {
            throw GCN_EXCEPTION("Another widget already has modal input focus.");
        }

        mModalMouseInputFocusedWidget = widget;
    }

    void FocusHandler::releaseModalFocus(std::shared_ptr<ZWidget> widget)
    {
        if (mModalFocusedWidget == widget)
        {
            mModalFocusedWidget = nullptr;
        }
    }

    void FocusHandler::releaseModalMouseInputFocus(std::shared_ptr<ZWidget> widget)
    {
        if (mModalMouseInputFocusedWidget == widget)
        {
            mModalMouseInputFocusedWidget = nullptr;
        }
    }

    std::shared_ptr<ZWidget> FocusHandler::getFocused() const
    {
        return mFocusedWidget;
    }

    std::shared_ptr<ZWidget> FocusHandler::getModalFocused() const
    {
        return mModalFocusedWidget;
    }

    std::shared_ptr<ZWidget> FocusHandler::getModalMouseInputFocused() const
    {
        return mModalMouseInputFocusedWidget;
    }

    void FocusHandler::focusNext()
    {
        int i;
        int focusedWidget = -1;
        for (i = 0; i < (int)mWidgets.size(); ++i)
        {
            if (mWidgets[i] == mFocusedWidget)
            {
                focusedWidget = i;
            }
        }
        int focused = focusedWidget;

        // i is a counter that ensures that the following loop
        // won't get stuck in an infinite loop
        i = (int)mWidgets.size();
        do
        {
            ++focusedWidget;

            if (i==0)
            {
                focusedWidget = -1;
                break;
            }

            --i;

            if (focusedWidget >= (int)mWidgets.size())
            {
                focusedWidget = 0;
            }

            if (focusedWidget == focused)
            {
                return;
            }
        }
        while (!mWidgets.at(focusedWidget)->isFocusable());

        if (focusedWidget >= 0)
        {
            mFocusedWidget = mWidgets.at(focusedWidget);

            Event focusEvent(mFocusedWidget);
            distributeFocusGainedEvent(focusEvent);
        }

        if (focused >= 0)
        {
            Event focusEvent(mWidgets.at(focused));
            distributeFocusLostEvent(focusEvent);
        }
    }

    void FocusHandler::focusPrevious()
    {
        if (mWidgets.size() == 0)
        {
            mFocusedWidget = nullptr;
            return;
        }

        int i;
        int focusedWidget = -1;
        for (i = 0; i < (int)mWidgets.size(); ++i)
        {
            if (mWidgets[i] == mFocusedWidget)
            {
                focusedWidget = i;
            }
        }
        int focused = focusedWidget;

        // i is a counter that ensures that the following loop
        // won't get stuck in an infinite loop
        i = (int)mWidgets.size();
        do
        {
            --focusedWidget;

            if (i==0)
            {
                focusedWidget = -1;
                break;
            }

            --i;

            if (focusedWidget <= 0)
            {
                focusedWidget = mWidgets.size() - 1;
            }

            if (focusedWidget == focused)
            {
                return;
            }
        }
        while (!mWidgets.at(focusedWidget)->isFocusable());

        if (focusedWidget >= 0)
        {
            mFocusedWidget = mWidgets.at(focusedWidget);
            Event focusEvent(mFocusedWidget);
            distributeFocusGainedEvent(focusEvent);
        }

        if (focused >= 0)
        {
            Event focusEvent(mWidgets.at(focused));
            distributeFocusLostEvent(focusEvent);
        }
    }

    bool FocusHandler::isFocused(std::shared_ptr<const ZWidget> widget) const
    {
        return mFocusedWidget == widget;
    }

    void FocusHandler::add(std::shared_ptr<ZWidget> widget)
    {
        mWidgets.push_back(widget);
    }

    void FocusHandler::remove(std::shared_ptr<ZWidget> widget)
    {
        if (isFocused(widget))
        {
            mFocusedWidget = nullptr;
        }

        auto iter=std::find_if(mWidgets.begin(),mWidgets.end(),[&widget](std::shared_ptr<ZWidget> const& item)
        {
            return item==widget;
        });

        if (iter!=mWidgets.end())
            mWidgets.erase(iter);

        if (mDraggedWidget == widget)
        {
            mDraggedWidget = nullptr;
            return;
        }

        if (mLastWidgetWithMouse == widget)
        {
            mLastWidgetWithMouse = nullptr;
            return;
        }

        if (mLastWidgetWithModalFocus == widget)
        {
            mLastWidgetWithModalFocus = nullptr;
            return;
        }

        if (mLastWidgetWithModalMouseInputFocus == widget)
        {
            mLastWidgetWithModalMouseInputFocus = nullptr;
            return;
        }

        if (mLastWidgetPressed == widget)
        {
            mLastWidgetPressed = nullptr;
            return;
        }
    }

    void FocusHandler::focusNone()
    {
        if (mFocusedWidget != nullptr)
        {
            auto focused = mFocusedWidget;
            mFocusedWidget = nullptr;

            Event focusEvent(focused);
            distributeFocusLostEvent(focusEvent);
        }
    }

    void FocusHandler::tabNext()
    {
        if (mFocusedWidget != nullptr)
        {
            if (!mFocusedWidget->isTabOutEnabled())
            {
                return;
            }
        }

        if (mWidgets.size() == 0)
        {
            mFocusedWidget = nullptr;
            return;
        }

        int i;
        int focusedWidget = -1;
        for (i = 0; i < (int)mWidgets.size(); ++i)
        {
            if (mWidgets[i] == mFocusedWidget)
            {
                focusedWidget = i;
            }
        }
        int focused = focusedWidget;
        bool done = false;

        // i is a counter that ensures that the following loop
        // won't get stuck in an infinite loop
        i = (int)mWidgets.size();
        do
        {
            ++focusedWidget;

            if (i==0)
            {
                focusedWidget = -1;
                break;
            }

            --i;

            if (focusedWidget >= (int)mWidgets.size())
            {
                focusedWidget = 0;
            }

            if (focusedWidget == focused)
            {
                return;
            }

            if (mWidgets.at(focusedWidget)->isFocusable() &&
                mWidgets.at(focusedWidget)->isTabInEnabled() &&
                (mModalFocusedWidget == nullptr ||
                 mWidgets.at(focusedWidget)->isModalFocused()))
            {
                done = true;
            }
        }
        while (!done);

        if (focusedWidget >= 0)
        {
            mFocusedWidget = mWidgets.at(focusedWidget);
            Event focusEvent(mFocusedWidget);
            distributeFocusGainedEvent(focusEvent);
        }

        if (focused >= 0)
        {
            Event focusEvent(mWidgets.at(focused));
            distributeFocusLostEvent(focusEvent);
        }
    }

    void FocusHandler::tabPrevious()
    {
        if (mFocusedWidget != nullptr)
        {
            if (!mFocusedWidget->isTabOutEnabled())
            {
                return;
            }
        }

        if (mWidgets.size() == 0)
        {
            mFocusedWidget = nullptr;
            return;
        }

        int i;
        int focusedWidget = -1;
        for (i = 0; i < (int)mWidgets.size(); ++i)
        {
            if (mWidgets[i] == mFocusedWidget)
            {
                focusedWidget = i;
            }
        }
        int focused = focusedWidget;
        bool done = false;

        // i is a counter that ensures that the following loop
        // won't get stuck in an infinite loop
        i = (int)mWidgets.size();
        do
        {
            --focusedWidget;

            if (i==0)
            {
                focusedWidget = -1;
                break;
            }

            --i;

            if (focusedWidget <= 0)
            {
                focusedWidget = mWidgets.size() - 1;
            }

            if (focusedWidget == focused)
            {
                return;
            }

            if (mWidgets.at(focusedWidget)->isFocusable() &&
                mWidgets.at(focusedWidget)->isTabInEnabled() &&
                (mModalFocusedWidget == nullptr ||
                 mWidgets.at(focusedWidget)->isModalFocused()))
            {
                done = true;
            }
        }
        while (!done);

        if (focusedWidget >= 0)
        {
            mFocusedWidget = mWidgets.at(focusedWidget);
            Event focusEvent(mFocusedWidget);
            distributeFocusGainedEvent(focusEvent);
        }

        if (focused >= 0)
        {
            Event focusEvent(mWidgets.at(focused));
            distributeFocusLostEvent(focusEvent);
        }
    }

    void FocusHandler::distributeFocusLostEvent(const Event& focusEvent)
    {
        auto sourceWidget = focusEvent.getSource();

        // Send the event to all focus listeners of the widget.
        auto focusListeners = sourceWidget->getFocusListeners();
        for (auto listener : focusListeners)
            listener->focusLost(focusEvent);
    }

    void FocusHandler::distributeFocusGainedEvent(const Event& focusEvent)
    {
        auto sourceWidget = focusEvent.getSource();

        // Send the event to all focus listeners of the widget.
        auto focusListeners = sourceWidget->getFocusListeners();
        for (auto listener : focusListeners)
            listener->focusGained(focusEvent);
    }

    std::shared_ptr<ZWidget> FocusHandler::getDraggedWidget()
    {
        return mDraggedWidget;
    }

    void FocusHandler::setDraggedWidget(std::shared_ptr<ZWidget> draggedWidget)
    {
        mDraggedWidget = draggedWidget;
    }

    std::shared_ptr<ZWidget> FocusHandler::getLastWidgetWithMouse()
    {
        return mLastWidgetWithMouse;
    }

    void FocusHandler::setLastWidgetWithMouse(std::shared_ptr<ZWidget> lastWidgetWithMouse)
    {
        mLastWidgetWithMouse = lastWidgetWithMouse;
    }

    std::shared_ptr<ZWidget> FocusHandler::getLastWidgetWithModalFocus()
    {
        return mLastWidgetWithModalFocus;
    }

    void FocusHandler::setLastWidgetWithModalFocus(std::shared_ptr<ZWidget> lastWidgetWithModalFocus)
    {
        mLastWidgetWithModalFocus = lastWidgetWithModalFocus;
    }

    std::shared_ptr<ZWidget> FocusHandler::getLastWidgetWithModalMouseInputFocus()
    {
        return mLastWidgetWithModalMouseInputFocus;
    }

    void FocusHandler::setLastWidgetWithModalMouseInputFocus(std::shared_ptr<ZWidget> lastWidgetWithModalMouseInputFocus)
    {
        mLastWidgetWithModalMouseInputFocus = lastWidgetWithModalMouseInputFocus;
    }

    std::shared_ptr<ZWidget> FocusHandler::getLastWidgetPressed()
    {
        return mLastWidgetPressed;
    }

    void FocusHandler::setLastWidgetPressed(std::shared_ptr<ZWidget> lastWidgetPressed)
    {
        mLastWidgetPressed = lastWidgetPressed;
    }
}