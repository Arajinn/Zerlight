//
// Created by tatiana on 26.09.18.
//

#ifndef GUICHAN_WIDGETLISTENER_H
#define GUICHAN_WIDGETLISTENER_H

namespace gui {
    class Event;
    /**
     * Interface for listening for events from widgets. When a widget's size,
     * location or visibility changes, the relevant method of the listener is
     * invoked.
     *
     * @see Widget::addWidgetListener, Widget::removeWidgetListener
     * @author Olof Naess�n
     * @since 0.8.0
     */
    class WidgetListener {
    public:

        /**
         * Destructor.
         */
        virtual ~WidgetListener();

        /**
         * Invoked when a widget changes its size.
         *
         * @param event Describes the event.
         * @since 0.8.0
         */
        virtual void widgetResized(const Event& event);

        /**
         * Invoked when a widget is moved.
         *
         * @param event Describes the event.
         * @since 0.8.0
         */
        virtual void widgetMoved(const Event& event);

        /**
         * Invoked when a widget is hidden, i.e it's set to be
         * not visible.
         *
         * @param event Describes the event.
         * @since 0.8.0
         */
        virtual void widgetHidden(const Event& event);

        /**
         * Invoked when a widget is shown, i.e it's set to be
         * visible.
         *
         * @param event Describes the event.
         * @since 0.8.0
         */
        virtual void widgetShown(const Event& event);

    protected:
        /**
         * Constructor.
         *
         * You should not be able to make an instance of WidgetListener,
         * therefore its constructor is protected.
         */
        WidgetListener();
    };
}

#endif //GUICHAN_WIDGETLISTENER_H
