//
// Created by tatiana on 28.09.18.
//

#include "Gui.h"
#include "ZWidget.h"
#include "Input.h"
#include "FocusHandler.h"
#include "Graphics.h"
#include "MouseEvent.h"
#include "MouseListener.h"
#include "KeyListener.h"
#include "Exception.h"

#include <algorithm>

namespace gui
{
    Gui::Gui()
            :mTop(nullptr),
             mGraphics(nullptr),
             mInput(nullptr),
             mTabbing(true),
             mShiftPressed(false),
             mMetaPressed(false),
             mControlPressed(false),
             mAltPressed(false),
             mLastMousePressButton(0),
             mLastMousePressTimeStamp(0),
             mLastMouseX(0),
             mLastMouseY(0),
             mClickCount(1),
             mLastMouseDragButton(0)
    {
        mFocusHandler = std::make_shared<FocusHandler>();
    }

    Gui::~Gui()
    {
        if (ZWidget::widgetExists(mTop))
        {
            setTop(nullptr);
        }

        mFocusHandler=nullptr;
    }

    void Gui::setTop(std::shared_ptr<ZWidget> top)
    {
        if (mTop != nullptr)
        {
            mTop->setFocusHandler(nullptr);
        }
        if (top != nullptr)
        {
            top->setFocusHandler(mFocusHandler);
        }

        mTop = top;
    }

    std::shared_ptr<ZWidget> Gui::getTop() const
    {
        return mTop;
    }

    void Gui::setGraphics(std::shared_ptr<Graphics> graphics)
    {
        mGraphics = graphics;
    }

    std::shared_ptr<Graphics> Gui::getGraphics() const
    {
        return mGraphics;
    }

    void Gui::setInput(std::shared_ptr<Input> input)
    {
        mInput = input;
    }

    std::shared_ptr<Input> Gui::getInput() const
    {
        return mInput;
    }

    void Gui::logic()
    {
        if (mTop == nullptr)
            throw GCN_EXCEPTION("No top widget set");

        handleModalFocus();
        handleModalMouseInputFocus();

        if (mInput != nullptr)
        {
            mInput->pollInput();

            handleKeyInput();
            handleMouseInput();
        }

        mTop->logic();
    }

    void Gui::draw()
    {
        if (mTop == nullptr)
            throw GCN_EXCEPTION("No top widget set");

        if (mGraphics == nullptr)
            throw GCN_EXCEPTION("No graphics set");

        if (!mTop->isVisible())
            return;

        mGraphics->beginDraw();
        mTop->draw(mGraphics);
        mGraphics->endDraw();
    }

    void Gui::focusNone()
    {
        mFocusHandler->focusNone();
    }

    void Gui::setTabbingEnabled(bool tabbing)
    {
        mTabbing = tabbing;
    }

    bool Gui::isTabbingEnabled()
    {
        return mTabbing;
    }

    void Gui::addGlobalKeyListener(std::shared_ptr<KeyListener> keyListener)
    {
        mKeyListeners.push_back(keyListener);
    }

    void Gui::removeGlobalKeyListener(std::shared_ptr<KeyListener>  keyListener)
    {
        mKeyListeners.remove(keyListener);
    }

    void Gui::handleMouseInput()
    {
        while (!mInput->isMouseQueueEmpty())
        {
            MouseInput mouseInput = mInput->dequeueMouseInput();
            //std::cout << "mouse event: " << mouseInput.getType() << std::endl;

            switch (mouseInput.getType())
            {
                case MouseInput::Pressed:
                    handleMousePressed(mouseInput);
                    break;
                case MouseInput::Released:
                    handleMouseReleased(mouseInput);
                    break;
                case MouseInput::Moved:
                    handleMouseMoved(mouseInput);
                    break;
                case MouseInput::WheelMovedDown:
                    handleMouseWheelMovedDown(mouseInput);
                    break;
                case MouseInput::WheelMovedUp:
                    handleMouseWheelMovedUp(mouseInput);
                    break;
                default:
                    throw GCN_EXCEPTION("Unknown mouse input type.");
                    break;
            }

            // Save the current mouse state. It's needed to send
            // mouse exited events and mouse entered events when
            // the mouse exits a widget and when a widget releases
            // modal mouse input focus.
            mLastMouseX = mouseInput.getX();
            mLastMouseY = mouseInput.getY();
        }
    }

    void Gui::handleKeyInput()
    {
        while (!mInput->isKeyQueueEmpty())
        {
            KeyInput keyInput = mInput->dequeueKeyInput();

            // Save modifiers state
            mShiftPressed = keyInput.isShiftPressed();
            mMetaPressed = keyInput.isMetaPressed();
            mControlPressed = keyInput.isControlPressed();
            mAltPressed = keyInput.isAltPressed();

            KeyEvent keyEventToGlobalKeyListeners(nullptr,
                                                  nullptr,
                                                  mShiftPressed,
                                                  mControlPressed,
                                                  mAltPressed,
                                                  mMetaPressed,
                                                  keyInput.getType(),
                                                  keyInput.isNumericPad(),
                                                  keyInput.getKey());

            distributeKeyEventToGlobalKeyListeners(keyEventToGlobalKeyListeners);

            // If a global key listener consumes the event it will not be
            // sent further to the source of the event.
            if (keyEventToGlobalKeyListeners.isConsumed())
            {
                continue;
            }

            bool keyEventConsumed = false;

            // Send key inputs to the focused widgets
            if (mFocusHandler->getFocused() != NULL)
            {
                auto source = getKeyEventSource();
                KeyEvent keyEvent(source,
                                  source,
                                  mShiftPressed,
                                  mControlPressed,
                                  mAltPressed,
                                  mMetaPressed,
                                  keyInput.getType(),
                                  keyInput.isNumericPad(),
                                  keyInput.getKey());


                if (!mFocusHandler->getFocused()->isFocusable())
                    mFocusHandler->focusNone();
                else
                    distributeKeyEvent(keyEvent);

                keyEventConsumed = keyEvent.isConsumed();
            }

            // If the key event hasn't been consumed and
            // tabbing is enable check for tab press and
            // change focus.
            if (!keyEventConsumed
                && mTabbing
                && keyInput.getKey().getValue() == Key::Tab
                && keyInput.getType() == KeyInput::Pressed)
            {
                if (keyInput.isShiftPressed())
                    mFocusHandler->tabPrevious();
                else
                    mFocusHandler->tabNext();
            }
        }
    }

    void Gui::handleMouseMoved(const MouseInput& mouseInput)
    {
        // Get tha last widgets with the mouse using the
        // last known mouse position.
        auto mLastWidgetsWithMouse = getWidgetsAt(mLastMouseX, mLastMouseY);

        // Check if the mouse has left the application window.
        if (mouseInput.getX() < 0
            || mouseInput.getY() < 0
            || !mTop->getDimension().isContaining(mouseInput.getX(), mouseInput.getY()))
        {
            for (auto widget : mLastWidgetsWithMouse)
            {
                distributeMouseEvent(widget,
                                     MouseEvent::Exited,
                                     mouseInput.getButton(),
                                     mouseInput.getX(),
                                     mouseInput.getY(),
                                     true,
                                     true);
            }
        }
            // The mouse is in the application window.
        else
        {
            // Calculate which widgets should receive a mouse exited event
            // and which should receive a mouse entered event by using the
            // last known mouse position and the latest mouse position.
            std::set<std::shared_ptr<ZWidget>> mWidgetsWithMouse = getWidgetsAt(mouseInput.getX(), mouseInput.getY());
            std::set<std::shared_ptr<ZWidget>> mWidgetsWithMouseExited;
            std::set<std::shared_ptr<ZWidget>> mWidgetsWithMouseEntered;
            std::set_difference(mLastWidgetsWithMouse.begin(), mLastWidgetsWithMouse.end(),
                    mWidgetsWithMouse.begin(), mWidgetsWithMouse.end(),
                    std::inserter(mWidgetsWithMouseExited, mWidgetsWithMouseExited.begin()));
            std::set_difference(mWidgetsWithMouse.begin(), mWidgetsWithMouse.end(),
                    mLastWidgetsWithMouse.begin(), mLastWidgetsWithMouse.end(),
                    std::inserter(mWidgetsWithMouseEntered, mWidgetsWithMouseEntered.begin()));

            for (auto widget : mWidgetsWithMouseExited)
            {
                distributeMouseEvent(widget,
                                     MouseEvent::Exited,
                                     mouseInput.getButton(),
                                     mouseInput.getX(),
                                     mouseInput.getY(),
                                     true,
                                     true);
                // As the mouse has exited a widget we need
                // to reset the click count and the last mouse
                // press time stamp.
                mClickCount = 1;
                mLastMousePressTimeStamp = 0;
            }

            for (auto widget : mWidgetsWithMouseEntered)
            {
                // If a widget has modal mouse input focus we
                // only want to send entered events to that widget
                // and the widget's parents.
                if ((mFocusHandler->getModalMouseInputFocused() != nullptr && widget->isModalMouseInputFocused())
                    || mFocusHandler->getModalMouseInputFocused() == nullptr)
                {
                    distributeMouseEvent(widget,
                                         MouseEvent::Entered,
                                         mouseInput.getButton(),
                                         mouseInput.getX(),
                                         mouseInput.getY(),
                                         true,
                                         true);
                }
            }
        }

        if (mFocusHandler->getDraggedWidget() != nullptr)
        {
            distributeMouseEvent(mFocusHandler->getDraggedWidget(),
                                 MouseEvent::Dragged,
                                 mLastMouseDragButton,
                                 mouseInput.getX(),
                                 mouseInput.getY());
        }
        else
        {
            auto sourceWidget = getMouseEventSource(mouseInput.getX(), mouseInput.getY());
            distributeMouseEvent(sourceWidget,
                                 MouseEvent::Moved,
                                 mouseInput.getButton(),
                                 mouseInput.getX(),
                                 mouseInput.getY());
        }
    }

    void Gui::handleMousePressed(const MouseInput& mouseInput)
    {
        auto sourceWidget = getMouseEventSource(mouseInput.getX(), mouseInput.getY());

        if (mFocusHandler->getDraggedWidget() != nullptr)
            sourceWidget = mFocusHandler->getDraggedWidget();

        int sourceWidgetX, sourceWidgetY;
        sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);

        if ((mFocusHandler->getModalFocused() != nullptr && sourceWidget->isModalFocused())
            || mFocusHandler->getModalFocused() == nullptr)
        {
            sourceWidget->requestFocus();
        }

        if (mouseInput.getTimeStamp() - mLastMousePressTimeStamp < 250
            && mLastMousePressButton == mouseInput.getButton())
            mClickCount++;
        else
            mClickCount = 1;

        distributeMouseEvent(sourceWidget,
                             MouseEvent::Pressed,
                             mouseInput.getButton(),
                             mouseInput.getX(),
                             mouseInput.getY());


        mFocusHandler->setLastWidgetPressed(sourceWidget);

        mFocusHandler->setDraggedWidget(sourceWidget);
        mLastMouseDragButton = mouseInput.getButton();

        mLastMousePressButton = mouseInput.getButton();
        mLastMousePressTimeStamp = mouseInput.getTimeStamp();
    }

    void Gui::handleMouseWheelMovedDown(const MouseInput& mouseInput)
    {
        auto sourceWidget = getMouseEventSource(mouseInput.getX(), mouseInput.getY());

        if (mFocusHandler->getDraggedWidget() != nullptr)
            sourceWidget = mFocusHandler->getDraggedWidget();

        int sourceWidgetX, sourceWidgetY;
        sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);

        distributeMouseEvent(sourceWidget,
                             MouseEvent::WheelMovedDown,
                             mouseInput.getButton(),
                             mouseInput.getX(),
                             mouseInput.getY());
    }

    void Gui::handleMouseWheelMovedUp(const MouseInput& mouseInput)
    {
        auto sourceWidget = getMouseEventSource(mouseInput.getX(), mouseInput.getY());

        if (mFocusHandler->getDraggedWidget() != nullptr)
            sourceWidget = mFocusHandler->getDraggedWidget();

        int sourceWidgetX, sourceWidgetY;
        sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);

        distributeMouseEvent(sourceWidget,
                             MouseEvent::WheelMovedUp,
                             mouseInput.getButton(),
                             mouseInput.getX(),
                             mouseInput.getY());
    }

    void Gui::handleMouseReleased(const MouseInput& mouseInput)
    {
        auto sourceWidget = getMouseEventSource(mouseInput.getX(), mouseInput.getY());

        if (mFocusHandler->getDraggedWidget() != nullptr)
        {
            if (sourceWidget != mFocusHandler->getLastWidgetPressed())
                mFocusHandler->setLastWidgetPressed(nullptr);

            sourceWidget = mFocusHandler->getDraggedWidget();
        }

        int sourceWidgetX, sourceWidgetY;
        sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);

        distributeMouseEvent(sourceWidget,
                             MouseEvent::Released,
                             mouseInput.getButton(),
                             mouseInput.getX(),
                             mouseInput.getY());

        if (mouseInput.getButton() == mLastMousePressButton
            && mFocusHandler->getLastWidgetPressed() == sourceWidget)
        {
            distributeMouseEvent(sourceWidget,
                                 MouseEvent::Clicked,
                                 mouseInput.getButton(),
                                 mouseInput.getX(),
                                 mouseInput.getY());

            mFocusHandler->setLastWidgetPressed(NULL);
        }
        else
        {
            mLastMousePressButton = 0;
            mClickCount = 0;
        }

        if (mFocusHandler->getDraggedWidget() != NULL)
            mFocusHandler->setDraggedWidget(NULL);
    }

    std::shared_ptr<ZWidget> Gui::getWidgetAt(int x, int y)
    {
        // If the widget's parent has no child then we have found the widget..
        auto parent = mTop;
        auto child = mTop;

        while (child != nullptr)
        {
            auto swap = child;
            int parentX, parentY;
            parent->getAbsolutePosition(parentX, parentY);
            child = parent->getWidgetAt(x - parentX, y - parentY);
            parent = swap;
        }

        return parent;
    }

    std::set<std::shared_ptr<ZWidget>> Gui::getWidgetsAt(int x, int y)
    {
        std::set<std::shared_ptr<ZWidget>> result;

        auto widget = mTop;

        while (widget != nullptr)
        {
            result.insert(widget);
            int absoluteX, absoluteY;
            widget->getAbsolutePosition(absoluteX, absoluteY);
            widget = widget->getWidgetAt(x - absoluteX, y - absoluteY);
        }

        return result;
    }

    std::shared_ptr<ZWidget> Gui::getMouseEventSource(int x, int y)
    {
        auto widget = getWidgetAt(x, y);

        if (mFocusHandler->getModalMouseInputFocused() != nullptr && !widget->isModalMouseInputFocused())
            return mFocusHandler->getModalMouseInputFocused();

        return widget;
    }

    std::shared_ptr<ZWidget> Gui::getKeyEventSource()
    {
        auto widget = mFocusHandler->getFocused();

        while (widget->getInternalFocusHandler() != nullptr && widget->getInternalFocusHandler()->getFocused() != nullptr)
        {
            widget = widget->getInternalFocusHandler()->getFocused();
        }

        return widget;
    }

    void Gui::distributeMouseEvent(std::shared_ptr<ZWidget> source,
                                   int type,
                                   int button,
                                   int x,
                                   int y,
                                   bool force,
                                   bool toSourceOnly)
    {
        auto parent = source;
        auto widget = source;

        if (mFocusHandler->getModalFocused() != nullptr && !widget->isModalFocused() && !force)
            return;

        if (mFocusHandler->getModalMouseInputFocused() != nullptr && !widget->isModalMouseInputFocused() && !force)
            return;

        MouseEvent mouseEvent(source,
                              source,
                              mShiftPressed,
                              mControlPressed,
                              mAltPressed,
                              mMetaPressed,
                              type,
                              button,
                              x,
                              y,
                              mClickCount);

        while (parent != nullptr)
        {
            // If the widget has been removed due to input
            // cancel the distribution.
            if (!ZWidget::widgetExists(widget))
                break;

            parent = widget->getParent();

            if (widget->isEnabled() || force)
            {
                int widgetX, widgetY;
                widget->getAbsolutePosition(widgetX, widgetY);

                mouseEvent.setX(x - widgetX);
                mouseEvent.setY(y - widgetY);
                mouseEvent.setDistributor(widget);
                auto mouseListeners = widget->getMouseListeners();

                auto event_type=mouseEvent.getType();

                //std::cout << "mouse event: " << event_type << " " << mouseEvent.getX() << " " << mouseEvent.getY() << std::endl;

                // Send the event to all mouse listeners of the widget.
                for (auto listener : mouseListeners)
                {
                    switch (event_type)
                    {
                        case MouseEvent::Entered:
                            listener->mouseEntered(mouseEvent);
                            break;
                        case MouseEvent::Exited:
                            listener->mouseExited(mouseEvent);
                            break;
                        case MouseEvent::Moved:
                            listener->mouseMoved(mouseEvent);
                            break;
                        case MouseEvent::Pressed:
                            listener->mousePressed(mouseEvent);
                            break;
                        case MouseEvent::Released:
                            listener->mouseReleased(mouseEvent);
                            break;
                        case MouseEvent::WheelMovedUp:
                            listener->mouseWheelMovedUp(mouseEvent);
                            break;
                        case MouseEvent::WheelMovedDown:
                            listener->mouseWheelMovedDown(mouseEvent);
                            break;
                        case MouseEvent::Dragged:
                            listener->mouseDragged(mouseEvent);
                            break;
                        case MouseEvent::Clicked:
                            listener->mouseClicked(mouseEvent);
                            break;
                        default:
                            throw GCN_EXCEPTION("Unknown mouse event type.");
                    }
                }

                if (toSourceOnly)
                    break;

            }

            auto swap = widget;
            widget = parent;
            parent = swap->getParent();

            // If a non modal focused widget has been reach
            // and we have modal focus cancel the distribution.
            if (mFocusHandler->getModalFocused() != nullptr && widget != nullptr && !widget->isModalFocused())
                break;

            // If a non modal mouse input focused widget has been reach
            // and we have modal mouse input focus cancel the distribution.
            if (mFocusHandler->getModalMouseInputFocused() != nullptr && widget != nullptr && !widget->isModalMouseInputFocused())
                break;
        }
    }

    void Gui::distributeKeyEvent(KeyEvent& keyEvent)
    {
        auto parent = keyEvent.getSource();
        auto widget = keyEvent.getSource();

        if (mFocusHandler->getModalFocused() != nullptr && !widget->isModalFocused())
            return;

        if (mFocusHandler->getModalMouseInputFocused() != nullptr && !widget->isModalMouseInputFocused())
            return;

        while (parent != nullptr)
        {
            // If the widget has been removed due to input
            // cancel the distribution.
            if (!ZWidget::widgetExists(widget))
                break;

            parent = widget->getParent();

            if (widget->isEnabled())
            {
                keyEvent.setDistributor(widget);
                auto keyListeners = widget->getKeyListeners();

                // Send the event to all key listeners of the source widget.
                for (auto listener : keyListeners)
                {
                    switch (keyEvent.getType())
                    {
                        case KeyEvent::Pressed:
                            listener->keyPressed(keyEvent);
                            break;
                        case KeyEvent::Released:
                            listener->keyReleased(keyEvent);
                            break;
                        default:
                            throw GCN_EXCEPTION("Unknown key event type.");
                    }
                }
            }

            auto swap = widget;
            widget = parent;
            parent = swap->getParent();

            // If a non modal focused widget has been reach
            // and we have modal focus cancel the distribution.
            if (mFocusHandler->getModalFocused() != nullptr && !widget->isModalFocused())
                break;
        }
    }

    void Gui::distributeKeyEventToGlobalKeyListeners(KeyEvent& keyEvent)
    {
        for (auto listener : mKeyListeners)
        {
            switch (keyEvent.getType())
            {
                case KeyEvent::Pressed:
                    listener->keyPressed(keyEvent);
                    break;
                case KeyEvent::Released:
                    listener->keyReleased(keyEvent);
                    break;
                default:
                    throw GCN_EXCEPTION("Unknown key event type.");
            }

            if (keyEvent.isConsumed())
                break;
        }
    }

    void Gui::handleModalMouseInputFocus()
    {
        // Check if modal mouse input focus has been gained by a widget.
        if ((mFocusHandler->getLastWidgetWithModalMouseInputFocus()
             != mFocusHandler->getModalMouseInputFocused())
            && (mFocusHandler->getLastWidgetWithModalMouseInputFocus() == nullptr))
        {
            handleModalFocusGained();
            mFocusHandler->setLastWidgetWithModalMouseInputFocus(mFocusHandler->getModalMouseInputFocused());
        }
            // Check if modal mouse input focus has been released.
        else if ((mFocusHandler->getLastWidgetWithModalMouseInputFocus()
                  != mFocusHandler->getModalMouseInputFocused())
                 && (mFocusHandler->getLastWidgetWithModalMouseInputFocus() != nullptr))
        {
            handleModalFocusReleased();
            mFocusHandler->setLastWidgetWithModalMouseInputFocus(nullptr);
        }
    }

    void Gui::handleModalFocus()
    {
        // Check if modal focus has been gained by a widget.
        if ((mFocusHandler->getLastWidgetWithModalFocus()
             != mFocusHandler->getModalFocused())
            && (mFocusHandler->getLastWidgetWithModalFocus() == nullptr))
        {
            handleModalFocusGained();
            mFocusHandler->setLastWidgetWithModalFocus(mFocusHandler->getModalFocused());
        }
            // Check if modal focus has been released.
        else if ((mFocusHandler->getLastWidgetWithModalFocus()
                  != mFocusHandler->getModalFocused())
                 && (mFocusHandler->getLastWidgetWithModalFocus() != nullptr))
        {
            handleModalFocusReleased();
            mFocusHandler->setLastWidgetWithModalFocus(nullptr);
        }
    }

    void Gui::handleModalFocusGained()
    {
        // Get all widgets at the last known mouse position
        // and send them a mouse exited event.
        auto widgetsWithMouse = getWidgetsAt(mLastMouseX, mLastMouseY);

        for (auto widget : widgetsWithMouse)
        {
            distributeMouseEvent(widget,
                                 MouseEvent::Exited,
                                 mLastMousePressButton,
                                 mLastMouseX,
                                 mLastMouseY,
                                 true,
                                 true);
        }

        mFocusHandler->setLastWidgetWithModalMouseInputFocus(mFocusHandler->getModalMouseInputFocused());
    }

    void Gui::handleModalFocusReleased()
    {
        // Get all widgets at the last known mouse position
        // and send them a mouse entered event.
        auto widgetsWithMouse = getWidgetsAt(mLastMouseX, mLastMouseY);

        for (auto widget : widgetsWithMouse)
        {
            distributeMouseEvent(widget,
                                 MouseEvent::Entered,
                                 mLastMousePressButton,
                                 mLastMouseX,
                                 mLastMouseY,
                                 false,
                                 true);
        }
    }
}