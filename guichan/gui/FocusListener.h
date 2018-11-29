//
// Created by tatiana on 26.09.18.
//

#ifndef GUICHAN_FOCUSLISTENER_H
#define GUICHAN_FOCUSLISTENER_H

#include "guichan/gui/FocusListener.h"
#include "guichan/gui/MultipleInheritable.h"

namespace gui {
    class Event;

    /**
     * Interface for listening for focus events from widgets.
     *
     * @see Widget::addFocusListener, Widget::removeFocusListener
     * @author Olof Naess�n
     * @since 0.7.0
     */
    class FocusListener : public inheritable_enable_shared_from_this<FocusListener>{
    public:

        /**
         * Destructor.
         */
        virtual ~FocusListener();

        /**
         * Called when a widget gains focus.
         *
         * @param event Discribes the event.
         */
        virtual void focusGained(const Event& event);

        /**
         * Called when a widget loses focus.
         *
         * @param event Discribes the event.
         */
        virtual void focusLost(const Event& event);

    protected:
        /**
         * Constructor.
         *
         * You should not be able to make an instance of FocusListener,
         * therefore its constructor is protected.
         */
        FocusListener();
    };
}

#endif //GUICHAN_FOCUSLISTENER_H
