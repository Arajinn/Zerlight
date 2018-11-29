//
// Created by tatiana on 26.09.18.
//

#ifndef GUICHAN_FOCUSHANDLER_H
#define GUICHAN_FOCUSHANDLER_H

#include <memory>
#include <vector>

namespace gui {
    class ZWidget;
    class Event;

    /**
     * Handles focus for widgets in a Gui. Each Gui has at least one
     * focus handler.

     * You will probably not use the focus handler directly as Widget
     * has functions that automatically uses the active focus handler.
     *
     * @see Widget::isFocus, Widget::isModalFocused,
     *      Widget::isModalMouseInputFocused, Widget::requestFocus,
     *      Widget::requestModalFocus, Widget::requestModalMouseInputFocus,
     *      Widget::releaseModalFocus, Widget::relaseModalMouseInputFocus,
     *      Widget::setFocusable, Widget::isFocusable, FocusListener
     *
     * @since 0.1.0
     */
    class FocusHandler {
    public:

        /**
         * Constructor.
         */
        FocusHandler();

        /**
         * Destructor.
         */
        virtual ~FocusHandler();

        /**
         * Requests focus for a widget. Focus will only be granted to a widget
         * if it's focusable and if no other widget has modal focus.
         * If a widget receives focus a focus event will be sent to the
         * focus listeners of the widget.
         *
         * @param widget The widget to request focus for.
         * @see isFocused, Widget::requestFocus
         */
        virtual void requestFocus(std::shared_ptr<ZWidget> widget);

        /**
         * Requests modal focus for a widget. Focus will only be granted
         * to a widget if it's focusable and if no other widget has modal
         * focus.
         *
         * @param widget The widget to request modal focus for.
         * @throws Exception when another widget already has modal focus.
         * @see releaseModalFocus, Widget::requestModalFocus
         */
        virtual void requestModalFocus(std::shared_ptr<ZWidget> widget);

        /**
         * Requests modal mouse input focus for a widget. Focus will only
         * be granted to a widget if it's focusable and if no other widget
         * has modal mouse input focus.
         *
         * Modal mouse input focus means no other widget then the widget with
         * modal mouse input focus will receive mouse input. The widget with
         * modal mouse input focus will also receive mouse input no matter what
         * the mouse input is or where the mouse input occurs.
         *
         * @param widget The widget to focus for modal mouse input focus.
         * @throws Exception when another widget already has modal mouse input
         *         focus.
         * @see releaseModalMouseInputFocus, Widget::requestModalMouseInputFocus
         */
        virtual void requestModalMouseInputFocus(std::shared_ptr<ZWidget> widget);

        /**
         * Releases modal focus if the widget has modal focus.
         * If the widget doesn't have modal focus no relase will occur.
         *
         * @param widget The widget to release modal focus for.
         * @see reuqestModalFocus, Widget::releaseModalFocus
         */
        virtual void releaseModalFocus(std::shared_ptr<ZWidget> widget);

        /**
         * Releases modal mouse input focus if the widget has modal mouse input
         * focus. If the widget doesn't have modal mouse input focus no relase
         * will occur.
         *
         * @param widget the widget to release modal mouse input focus for.
         * @see requestModalMouseInputFocus, Widget::releaseModalMouseInputFocus
         */
        virtual void releaseModalMouseInputFocus(std::shared_ptr<ZWidget> widget);

        /**
         * Checks if a widget is focused.
         *
         * @param widget The widget to check.
         * @return True if the widget is focused, false otherwise.
         * @see Widget::isFocused
         */
        virtual bool isFocused(std::shared_ptr<const ZWidget> widget) const;

        /**
         * Gets the widget with focus.
         *
         * @return The widget with focus. NULL if no widget has focus.
         */
        virtual std::shared_ptr<ZWidget> getFocused() const;

        /**
         * Gets the widget with modal focus.
         *
         * @return The widget with modal focus. NULL if no widget has
         *         modal focus.
         */
        virtual std::shared_ptr<ZWidget> getModalFocused() const;

        /**
         * Gets the widget with modal mouse input focus.
         *
         * @return The widget with modal mouse input focus. NULL if
         *         no widget has modal mouse input focus.
         */
        virtual std::shared_ptr<ZWidget> getModalMouseInputFocused() const;

        /**
         * Focuses the next widget added to a conainer.
         * If no widget has focus the first widget gets focus. The order
         * in which the widgets are focused is determined by the order
         * they were added to a container.
         *
         * @see focusPrevious
         */
        virtual void focusNext();

        /**
         * Focuses the previous widget added to a contaienr.
         * If no widget has focus the first widget gets focus. The order
         * in which the widgets are focused is determined by the order
         * they were added to a container.
         *
         * @see focusNext
         */
        virtual void focusPrevious();

        /**
         * Adds a widget to by handles by the focus handler.
         *
         * @param widget The widget to add.
         * @see remove
         */
        virtual void add(std::shared_ptr<ZWidget> widget);

        /**
         * Removes a widget from the focus handler.
         *
         * @param widget The widget to remove.
         * @see add
         */
        virtual void remove(std::shared_ptr<ZWidget> widget);

        /**
         * Focuses nothing. A focus event will also be sent to the
         * focused widget's focus listeners if a widget has focus.
         */
        virtual void focusNone();

        /**
         * Focuses the next widget which allows tabbing in unless
         * the current focused Widget disallows tabbing out.
         *
         * @see tabPrevious
         */
        virtual void tabNext();

        /**
         * Focuses the previous widget which allows tabbing in unless
         * current focused widget disallows tabbing out.
         *
         * @see tabNext
         */
        virtual void tabPrevious();

        /**
         * Gets the widget being dragged. Used by the Gui class to
         * keep track of the dragged widget.
         *
         * @return the widget being dragged.
         * @see setDraggedWidget
         */
        virtual std::shared_ptr<ZWidget> getDraggedWidget();

        /**
         * Sets the widget being dragged. Used by the Gui class to
         * keep track of the dragged widget.
         *
         * @param draggedWidget The widget being dragged.
         * @see getDraggedWidget
         */
        virtual void setDraggedWidget(std::shared_ptr<ZWidget> draggedWidget);

        /**
         * Gets the last widget with the mouse. Used by the Gui class
         * to keep track the last widget with the mouse.
         *
         * @return The last widget with the mouse.
         * @see setLastWidgetWithMouse
         */
        virtual std::shared_ptr<ZWidget> getLastWidgetWithMouse();

        /**
         * Sets the last widget with the mouse. Used by the Gui class
         * to keep track the last widget with the mouse.
         *
         * @param lastWidgetWithMouse The last widget with the mouse.
         * @see getLastWidgetWithMouse
         */
        virtual void setLastWidgetWithMouse(std::shared_ptr<ZWidget> lastWidgetWithMouse);

        /**
         * Gets the last widget with modal focus.
         *
         * @return The last widget with modal focus.
         * @see setLastWidgetWithModalFocus
         */
        virtual std::shared_ptr<ZWidget> getLastWidgetWithModalFocus();

        /**
         * Sets the last widget with modal focus.
         *
         * @param lastWidgetWithModalFocus The last widget with modal focus.
         * @see getLastWidgetWithModalFocus
         */
        virtual void setLastWidgetWithModalFocus(std::shared_ptr<ZWidget> lastWidgetWithModalFocus);

        /**
         * Gets the last widget with modal mouse input focus.
         *
         * @return The last widget with modal mouse input focus.
         * @see setLastWidgetWithModalMouseInputFocus
         */
        virtual std::shared_ptr<ZWidget> getLastWidgetWithModalMouseInputFocus();

        /**
         * Sets the last widget with modal mouse input focus.
         *
         * @param lastMouseWithModalMouseInputFocus The last widget with
         *                                          modal mouse input focus.
         * @see getLastWidgetWithModalMouseInputFocus
         */
        virtual void setLastWidgetWithModalMouseInputFocus(std::shared_ptr<ZWidget> lastWidgetWithModalMouseInputFocus);

        /**
         * Gets the last widget pressed. Used by the Gui class to keep track
         * of pressed widgets.
         *
         * @return The last widget pressed.
         * @see setLastWidgetPressed
         */
        virtual std::shared_ptr<ZWidget> getLastWidgetPressed();

        /**
         * Sets the last widget pressed. Used by the Gui class to keep track
         * of pressed widgets.
         *
         * @param lastWidgetPressed The last widget pressed.
         * @see getLastWidgetPressed
         */
        virtual void setLastWidgetPressed(std::shared_ptr<ZWidget> lastWidgetPressed);

    protected:
        /**
         * Distributes a focus lost event.
         *
         * @param focusEvent the event to distribute.
         * @since 0.7.0
         */
        virtual void distributeFocusLostEvent(const Event& focusEvent);

        /**
         * Distributes a focus gained event.
         *
         * @param focusEvent the event to distribute.
         * @since 0.7.0
         */
        virtual void distributeFocusGainedEvent(const Event& focusEvent);

        /**
         * Holds the widgets currently being handled by the
         * focus handler.
         */
        std::vector<std::shared_ptr<ZWidget>> mWidgets;

        /**
         * Holds the focused widget. NULL if no widget has focus.
         */
        std::shared_ptr<ZWidget> mFocusedWidget;

        /**
         * Holds the modal focused widget. NULL if no widget has
         * modal focused.
         */
        std::shared_ptr<ZWidget> mModalFocusedWidget;

        /**
         * Holds the modal mouse input focused widget. NULL if no widget
         * is being dragged.
         */
        std::shared_ptr<ZWidget> mModalMouseInputFocusedWidget;

        /**
         * Holds the dragged widget. NULL if no widget is
         * being dragged.
         */
        std::shared_ptr<ZWidget> mDraggedWidget;

        /**
         * Holds the last widget with the mouse.
         */
        std::shared_ptr<ZWidget> mLastWidgetWithMouse;

        /**
         * Holds the last widget with modal focus.
         */
        std::shared_ptr<ZWidget> mLastWidgetWithModalFocus;

        /**
         * Holds the last widget with modal mouse input focus.
         */
        std::shared_ptr<ZWidget> mLastWidgetWithModalMouseInputFocus;

        /**
         * Holds the last widget pressed.
         */
        std::shared_ptr<ZWidget> mLastWidgetPressed;
    };
}

#endif //GUICHAN_FOCUSHANDLER_H
